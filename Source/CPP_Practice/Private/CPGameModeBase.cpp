// Fill out your copyright notice in the Description page of Project Settings.


#include "CPGameModeBase.h"

#include "CPAttributeComponent.h"
#include "CPCharacter.h"
#include "CPPickUpBase.h"
#include "CPPlayerState.h"
#include "EngineUtils.h"
#include "AI/CPAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Algo/RandomShuffle.h"

static TAutoConsoleVariable CVarSpawnBots(TEXT("cp.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ACPGameModeBase::ACPGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	MaxPickupSpawnCount = 10;
}

void ACPGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ACPGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	if (SpawnPickupQuery)
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
			this,
			SpawnPickupQuery,
			this, // Querier
			EEnvQueryRunMode::AllMatching, // 获取所有符合的点
			nullptr); // 默认 Query Params

		if (QueryInstance)
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ACPGameModeBase::OnSpawnPickupQueryCompleted);
		}
	}
}

void ACPGameModeBase::KillAll()
{
	for (TActorIterator<ACPAICharacter> It(GetWorld()); It; ++It)
	{
		ACPAICharacter* Bot = *It;

		UCPAttributeComponent* AttributeComp =UCPAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); // @fixme: pass in player? for kill credit
		}
	}
}

void ACPGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning via cvar 'CVarSpawnBots'."));
		return;
	}
	
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ACPAICharacter> It(GetWorld()); It; ++It)
	{
		ACPAICharacter* Bot = *It;

		UCPAttributeComponent* AttributeComp =UCPAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NrOfAliveBots);

	float MaxBotCount = 10.0f;
	
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity, Skipping bor spawn."));
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ACPGameModeBase::OnSpawnBotQueryCompleted);
	}
	
}

void ACPGameModeBase::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void ACPGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		
		RestartPlayer(Controller);
	}
}

void ACPGameModeBase::OnSpawnPickupQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (!QueryInstance || QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("EQS failed to generate pickup spawn locations."));
		return;
	}

	TArray<FVector> Locations;
	QueryInstance->GetQueryResultsAsLocations(Locations);

	if (Locations.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("EQS returned 0 valid pickup spawn locations."));
		return;
	}

	Algo::RandomShuffle(Locations);
	
	UE_LOG(LogTemp, Log, TEXT("EQS returned %d pickup spawn locations."), Locations.Num());

	for (int32 i = 0; i < Locations.Num() && i < MaxPickupSpawnCount; i++)
	{
		if (PickupClasses.Num() == 0)
		{
			break;
		}

		// 随机选一个拾取类（Coin or Potion）
		int32 ClassIndex = FMath::RandRange(0, PickupClasses.Num() - 1);
		TSubclassOf<AActor> PickupClass = PickupClasses[ClassIndex];

		FVector SpawnLocation = Locations[i];

		// 向下射线检测，自动调整高度
		FHitResult HitResult;
		FVector Start = SpawnLocation + FVector(0.f, 0.f, 500.f);   // 起点高于原点500单位
		FVector End = SpawnLocation - FVector(0.f, 0.f, 1000.f);    // 向下1000单位

		FCollisionQueryParams Params;
		Params.bTraceComplex = true;
		Params.bReturnPhysicalMaterial = false;

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, Params);

		if (bHit)
		{
			SpawnLocation.Z = HitResult.Location.Z;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Pickup spawn raycast did not hit the ground, using original location."));
		}

		// 先生成物体，位置先用 SpawnLocation
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(PickupClass, SpawnLocation, FRotator::ZeroRotator);

		// 如果生成成功且是 PickupBase 子类，读取底部偏移，调整位置
		if (SpawnedActor)
		{
			if (ACPPickUpBase* PickupBase = Cast<ACPPickUpBase>(SpawnedActor))
			{
				float BottomOffset = PickupBase->GetBottomOffset();
				FVector AdjustedLocation = SpawnLocation;
				AdjustedLocation.Z += -BottomOffset;  // 底部偏移一般是负数，所以减去

				SpawnedActor->SetActorLocation(AdjustedLocation);
			}
		}
	}
}

void ACPGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	if (ACPCharacter* Player = Cast<ACPCharacter>(VictimActor))
	{
		// 玩家死亡 → 延迟重生
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}
	else if (ACPAICharacter* AIPawn = Cast<ACPAICharacter>(VictimActor))
	{
		// AI 小兵被击杀 → 奖励击杀者
		APawn* KillerPawn = Cast<APawn>(Killer);
		if (KillerPawn)
		{
			if (ACPPlayerState* CPPlayerState = KillerPawn->GetPlayerState<ACPPlayerState>())
			{
				float RewardAmount = AIPawn->GetCredit();
				CPPlayerState->AddCredits(RewardAmount);

				UE_LOG(LogTemp, Log, TEXT("Granted %.2f credits to %s for killing AI"), RewardAmount, *CPPlayerState->GetPlayerName());
			}
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

