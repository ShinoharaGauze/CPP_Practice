// Fill out your copyright notice in the Description page of Project Settings.


#include "CPActionComponent.h"

#include "CPAction.h"
#include "CPP_Practice/CPP_Practice.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

UCPActionComponent::UCPActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UCPActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Server Only
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UCPAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
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
		
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));
		
		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void UCPActionComponent::AddAction(AActor* Instigator, TSubclassOf<UCPAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempt to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	UCPAction* NewAction = NewObject<UCPAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		
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
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}
				
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

void UCPActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

bool UCPActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UCPAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UCPActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCPActionComponent, Actions);
}