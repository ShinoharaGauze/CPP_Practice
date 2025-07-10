// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPPlayerController.generated.h"

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn); 

/**
 * 
 */
UCLASS()
class CPP_PRACTICE_API ACPPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	virtual void SetPawn(APawn* InPawn) override;
	
};
