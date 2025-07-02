// Fill out your copyright notice in the Description page of Project Settings.


#include "CPSprintGiver.h"

#include "CPActionComponent.h"

ACPSprintGiver::ACPSprintGiver()
{
	ACPSprintGiver::SetActiveState(true); // 初始为可交互
}

void ACPSprintGiver::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!bIsActive)
	{
		return;
	}

	UCPActionComponent* ActionComp = Cast<UCPActionComponent>(InstigatorPawn->GetComponentByClass(UCPActionComponent::StaticClass()));
	if (ActionComp)
	{
		ActionComp->AddAction(InstigatorPawn, SprintActionClass);
		SetActiveState(false);
	}
	
}

bool ACPSprintGiver::CanInteract_Implementation(APawn* InstigatorPawn) const
{
	if (!bIsActive)
	{
		return false;
	}

	UCPActionComponent* ActionComp = Cast<UCPActionComponent>(InstigatorPawn->GetComponentByClass(UCPActionComponent::StaticClass()));
	if (ActionComp && ActionComp->HasAction(SprintActionClass))
	{
		return false;
	}

	return true;
}
