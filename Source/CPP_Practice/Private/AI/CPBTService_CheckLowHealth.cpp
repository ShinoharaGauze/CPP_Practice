// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CPBTService_CheckLowHealth.h"

#include "AIController.h"
#include "CPAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCPBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ensure(MyController))
		{
			APawn* AIPawn = MyController->GetPawn();
			if ensure((AIPawn))
			{
				UCPAttributeComponent* AttributeComp = AIPawn->FindComponentByClass<UCPAttributeComponent>();

				const float HealthPercent = AttributeComp->GetHealth() / AttributeComp->GetHealthMax();

				const bool bIsLowHealth = HealthPercent <= 0.3f;

				BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bIsLowHealth);
			}
		}
	}
}
