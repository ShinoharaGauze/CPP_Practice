// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPBaseProjectile.h"
#include "GameplayTagContainer.h"
#include "CPMagicProjectile.generated.h"

class UCPAction_Effect;
/**
 * 
 */
UCLASS()
class CPP_PRACTICE_API ACPMagicProjectile : public ACPBaseProjectile
{
	GENERATED_BODY()

public:
	ACPMagicProjectile();

protected:
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UCPAction_Effect> BurningActionClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;
};
