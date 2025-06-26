// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CPAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "CPAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ACPAICharacter::ACPAICharacter()
{
	AttributeComp = CreateDefaultSubobject<UCPAttributeComponent>("AttributeComp");
	
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TimeToHitParamName = "TimeToHit";
}

void ACPAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AttributeComp->SetHealthMax(10.0f, true); // 设置AI最大血量为80，并重置当前血量
	AttributeComp->OnHealthChanged.AddDynamic(this, &ACPAICharacter::OnHealthChanged);
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ACPAICharacter::OnPawnSeen);
}

void ACPAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
 	if (AIC)
 	{
 		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
 	}
}


void ACPAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);

	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}

void ACPAICharacter::OnHealthChanged(AActor* InstigatorActor, UCPAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		
		if (NewHealth <= 0.0f)
		{
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			
			SetLifeSpan(10.0f);
		}
	}
}

