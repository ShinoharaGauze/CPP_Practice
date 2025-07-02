// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPickUpBase.h"
#include "CPCoin.generated.h"

/**
 * 
 */
UCLASS()
class CPP_PRACTICE_API ACPCoin : public ACPPickUpBase
{
	GENERATED_BODY()

public:
	ACPCoin();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	float CreditAmount = 25.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	float CooldownTime = 5.0f;
	
	FTimerHandle TimerHandle_Respawn;
	
};
