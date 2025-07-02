// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPlayerState.h"

ACPPlayerState::ACPPlayerState()
{
	Credits = 0.0f;
}

void ACPPlayerState::AddCredits(float Delta)
{
	if (!ensure(Delta != 0.0f))
	{
		return;
	}
	
	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
}