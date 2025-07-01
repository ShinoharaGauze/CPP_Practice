// Fill out your copyright notice in the Description page of Project Settings.


#include "CPActionComponent.h"

#include "CPAction.h"

UCPActionComponent::UCPActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCPActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UCPAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}

void UCPActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UCPActionComponent::AddAction(TSubclassOf<UCPAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UCPAction* NewAction = NewObject<UCPAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UCPActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UCPAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UCPActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UCPAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}
