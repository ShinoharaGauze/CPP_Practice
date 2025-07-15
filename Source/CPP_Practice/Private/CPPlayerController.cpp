// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPlayerController.h"

#include "Blueprint/UserWidget.h"

void ACPPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		return;
	}
	
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void ACPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ACPPlayerController::TogglePauseMenu);
}

void ACPPlayerController::SetPawn(APawn* Inpawn)
{
	Super::SetPawn(Inpawn);

	OnPawnChanged.Broadcast(Inpawn);
}

void ACPPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	BlueprintBeginPlayingState();
}
