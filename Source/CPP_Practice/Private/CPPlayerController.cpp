// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPlayerController.h"

void ACPPlayerController::SetPawn(APawn* Inpawn)
{
	Super::SetPawn(Inpawn);

	OnPawnChanged.Broadcast(Inpawn);
}