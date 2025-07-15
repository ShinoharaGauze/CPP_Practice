// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPlayerState.h"

#include "CPSaveGame.h"
#include "Net/UnrealNetwork.h"

ACPPlayerState::ACPPlayerState()
{
	Credits = 0.0f;
}

void ACPPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ACPPlayerState::AddCredits(float Delta)
{
	if (!HasAuthority()) return;
	
	if (!ensure(Delta != 0.0f))
	{
		return;
	}
	
	Credits += Delta;
	
	OnRep_Credits(Credits);
}

void ACPPlayerState::SavePlayerState_Implementation(UCPSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}

void ACPPlayerState::LoadPlayerState_Implementation(UCPSaveGame* SaveObject)
{
	if (SaveObject)
	{
		AddCredits(SaveObject->Credits);
	}
}

void ACPPlayerState::OnRep_Credits(const float OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

void ACPPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPPlayerState, Credits);
}
