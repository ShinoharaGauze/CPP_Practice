// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPickUpBase.h"
#include "CPHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class CPP_PRACTICE_API ACPHealthPotion : public ACPPickUpBase
{
	GENERATED_BODY()

public:
	ACPHealthPotion();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual bool CanInteract_Implementation(APawn* InstigatorPawn) const override;

protected:
	// 冷却时间（秒）
	UPROPERTY(EditDefaultsOnly, Category = "Potion")
	float CooldownTime = 10.0f;

	// 回复的生命值
	UPROPERTY(EditDefaultsOnly, Category = "Potion")
	float HealAmount = 50.0f;

	FTimerHandle TimerHandle_Respawn;
	
	virtual void SetActiveState(bool bNewActive) override;
	
	void Respawn();
};
