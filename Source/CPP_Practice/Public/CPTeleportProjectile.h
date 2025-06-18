// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPBaseProjectile.h"
#include "CPTeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class CPP_PRACTICE_API ACPTeleportProjectile : public ACPBaseProjectile
{
	GENERATED_BODY()

public:
	ACPTeleportProjectile();

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;
	
	FTimerHandle TimerHandle_DelayedDetonate;

	virtual void Explode_Implementation() override;
	
	void TeleportInstigator();

	virtual void BeginPlay() override;

};
