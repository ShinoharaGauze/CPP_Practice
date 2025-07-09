// Fill out your copyright notice in the Description page of Project Settings.


#include "CPActionComponent.h"

#include "CPAction.h"
#include "CPP_Practice/CPP_Practice.h"

UCPActionComponent::UCPActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UCPActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UCPAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void UCPActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + ": " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	for (UCPAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s : IsRunning: %s : Outer: %s"),
			*GetNameSafe(GetOwner()),
			*Action->ActionName.ToString(),
			Action->IsRunning() ? TEXT("true") : TEXT("false"),
			*GetNameSafe(GetOuter()));
		
		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void UCPActionComponent::AddAction(AActor* Instigator, TSubclassOf<UCPAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UCPAction* NewAction = NewObject<UCPAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UCPActionComponent::RemoveAction(UCPAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	
	Actions.Remove(ActionToRemove);
}

bool UCPActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UCPAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}

			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}
			
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
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

bool UCPActionComponent::HasAction(TSubclassOf<UCPAction> ActionClass) const
{
	for (UCPAction* Action : Actions)
	{
		if (Action && Action->GetClass() == ActionClass)
		{
			return true;
		}
	}
	return false;
}

void UCPActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName); 
}
