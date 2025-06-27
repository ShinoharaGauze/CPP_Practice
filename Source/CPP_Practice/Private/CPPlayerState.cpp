// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPlayerState.h"

ACPPlayerState::ACPPlayerState()
{
	Credits = 0.0f;
}

void ACPPlayerState::AddCredits(float Delta)
{
	Credits += Delta;

	OnCreditsChanged.Broadcast(Credits, Delta);
}