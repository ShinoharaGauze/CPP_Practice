// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPlayerState.h"

#include "Net/UnrealNetwork.h"

ACPPlayerState::ACPPlayerState()
{
	Credits = 0.0f;
}

void ACPPlayerState::BeginPlay()
{
	Super::BeginPlay();

	OldCredits = Credits;
}

void ACPPlayerState::AddCredits(float Delta)
{
	if (!HasAuthority()) return;
	
	if (!ensure(Delta != 0.0f))
	{
		return;
	}
	
	Credits += Delta;
	
	OnRep_Credits();
}

void ACPPlayerState::OnRep_Credits()
{
	float Delta = Credits - OldCredits;

	OnCreditsChanged.Broadcast(this, Credits, Delta);

	OldCredits = Credits;
}

void ACPPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPPlayerState, Credits);
}
