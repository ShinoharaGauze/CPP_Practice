// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CPPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ACPPlayerState*, PlayerState, float, NewCredits, float, Delta);

/**
 * 
 */
UCLASS()
class CPP_PRACTICE_API ACPPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACPPlayerState();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Credits")
	float Credits;

public:
	UPROPERTY(BlueprintAssignable, Category = "Credits")
	FOnCreditsChanged OnCreditsChanged;
	
	/** 获取当前积分 */
	UFUNCTION(BlueprintCallable, Category = "Credits")
	float GetCredits() const { return Credits; }

	/** 增加积分 */
	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(float Delta);
};
