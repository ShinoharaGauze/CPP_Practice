// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CPBTTask_RangedAttack.h"
#include "AIController.h"
#include "CPActionComponent.h"
#include "CPAction_Effect.h"
#include "CPAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UCPBTTask_RangedAttack::UCPBTTask_RangedAttack()
{
	MaxBulletSpread = 3.0f;
}

EBTNodeResult::Type UCPBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		if (!UCPAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}

		UCPActionComponent* TargetActionComp = Cast<UCPActionComponent>(TargetActor->GetComponentByClass(UCPActionComponent::StaticClass()));

		if (TargetActionComp)
		{
			if (TargetActionComp->HasAction(TSubclassOf<UCPAction>(BurningActionClass)))
			{
				TargetActionComp->AddAction(MyPawn, TSubclassOf<UCPAction>(StunnedActionClass));
			}
		}
		
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = MyPawn;

		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);

		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}



