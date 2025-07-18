// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CPAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "CPActionComponent.h"
#include "CPAttributeComponent.h"
#include "CPWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ACPAICharacter::ACPAICharacter()
{
	AttributeComp = CreateDefaultSubobject<UCPAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<UCPActionComponent>("ActionComp");
	
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	TimeToHitParamName = "TimeToHit";
}

void ACPAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AttributeComp->SetHealthMax(10.0f, true); // 设置AI最大血量为80，并重置当前血量
	AttributeComp->SetRageMax(0.0f, true);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ACPAICharacter::OnHealthChanged);
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ACPAICharacter::OnPawnSeen);
}

void ACPAICharacter::SetTargetActor(AActor* NewTarget)
{
	if (NewTarget == nullptr) return;
	
	AAIController* AIC = Cast<AAIController>(GetController());
 	if (AIC)
 	{
 		UBlackboardComponent* BlackboardComp = AIC->GetBlackboardComponent();
 		if (BlackboardComp->GetValueAsObject("TargetActor") == NewTarget)
 		{
 			return; // 重复目标，跳过
 		}
 		
 		BlackboardComp->SetValueAsObject("TargetActor", NewTarget);
 		
 		MulticastShowSpottedWidget();
 	}
}


void ACPAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
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

		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UCPWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
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

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			
			SetLifeSpan(10.0f);
		}
	}
}

void ACPAICharacter::MulticastShowSpottedWidget_Implementation()
{
	if (ActiveSpottedWidget == nullptr && SpottedWidgetClass)
	{
		ActiveSpottedWidget = CreateWidget<UCPWorldUserWidget>(GetWorld(), SpottedWidgetClass);
		if (ActiveSpottedWidget)
		{
			ActiveSpottedWidget->AttachedActor = this;
			ActiveSpottedWidget->AddToViewport(); 
		}
	}
}

