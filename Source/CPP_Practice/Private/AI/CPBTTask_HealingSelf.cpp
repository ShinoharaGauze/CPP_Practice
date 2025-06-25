// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CPBTTask_HealingSelf.h"
#include "AIController.h"
#include "CPAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UCPBTTask_HealingSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		
		if (UCPAttributeComponent* AttributeComp = Cast<UCPAttributeComponent>(
			MyPawn->GetComponentByClass(UCPAttributeComponent::StaticClass())))
		{
			bool bHealed = AttributeComp->ApplyHealthChange(+10.0f);
			
			return bHealed ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		}
		
	}

	return EBTNodeResult::Failed;
}
