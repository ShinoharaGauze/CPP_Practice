// Fill out your copyright notice in the Description page of Project Settings.


#include "CPGameModeBase.h"

#include "CPActionComponent.h"
#include "CPAttributeComponent.h"
#include "CPCharacter.h"
#include "CPMonsterData.h"
#include "CPPickUpBase.h"
#include "CPPlayerState.h"
#include "CPSaveGame.h"
#include "EngineUtils.h"
#include "AI/CPAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Algo/RandomShuffle.h"
#include "CPP_Practice/CPP_Practice.h"
#include "Engine/AssetManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable CVarSpawnBots(TEXT("cp.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ACPGameModeBase::ACPGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	MaxPickupSpawnCount = 20;

	SlotName = "SaveGame01";
}

void ACPGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
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

void ACPGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	ACPPlayerState* PlayerState = NewPlayer->GetPlayerState<ACPPlayerState>();
	if (PlayerState)
	{
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
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
		if (MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);
			
			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

			UAssetManager* Manager = UAssetManager::GetIfInitialized();
			if (Manager)
			{
				LogOnScreen(this, "Loading monster...", FColor::Green);
				
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ACPGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0]);
				Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
			}
		}
	}
}

void ACPGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedID, FVector SpawnLocation)
{
	LogOnScreen(this, "Finished loading.", FColor::Green);
	
	UAssetManager* Manager = UAssetManager::GetIfInitialized();
	if (Manager)
	{
		UCPMonsterData* MonsterData = Cast<UCPMonsterData>(Manager->GetPrimaryAssetObject(LoadedID));
		if (MonsterData)
		{
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
			if (NewBot)
			{
				LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));

				UCPActionComponent* ActionComp = Cast<UCPActionComponent>(NewBot->GetComponentByClass(UCPActionComponent::StaticClass()));
				if (ActionComp)
				{
					for (TSubclassOf<UCPAction> ActionClass : MonsterData->Actions)
					{
						ActionComp->AddAction(NewBot, ActionClass);
					}
				}
			}
		}
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

void ACPGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ACPPlayerState* PlayerState = Cast<ACPPlayerState>(GameState->PlayerArray[i]);
		if (PlayerState)
		{
			PlayerState->SavePlayerState(CurrentSaveGame);
			break;
		}
	}

	CurrentSaveGame->SavedActors.Empty(); 
	
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<UCPGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		FMemoryWriter MemWriter(ActorData.ByteData);
		
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		
		// Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		
		// Converts Actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar); 
		
		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ACPGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UCPSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<UCPGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);
		
					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
					
					Actor->Serialize(Ar);

					ICPGameplayInterface::Execute_OnActorLoaded(Actor);
					
					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<UCPSaveGame>(UGameplayStatics::CreateSaveGameObject(UCPSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}