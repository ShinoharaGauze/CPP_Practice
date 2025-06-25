// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CPAICharacter.h"

#include "AIController.h"
#include "CPAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ACPAICharacter::ACPAICharacter()
{
	AttributeComp = CreateDefaultSubobject<UCPAttributeComponent>("AttributeComp");
	
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
}

void ACPAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AttributeComp->SetHealthMax(10.0f, true); // 设置AI最大血量为80，并重置当前血量
	AttributeComp->OnHealthChanged.AddDynamic(this, &ACPAICharacter::OnHealthChanged);
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ACPAICharacter::OnPawnSeen);
}

void ACPAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();

		BBComp->SetValueAsObject("TargetActor", Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	}
}

void ACPAICharacter::OnHealthChanged(AActor* InstigatorActor, UCPAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
			FString::Printf(TEXT("[AI] %s took %.0f damage, remaining %.0f HP"),
				*GetName(), -Delta, NewHealth));
	}
}

