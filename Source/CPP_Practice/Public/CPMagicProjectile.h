// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPBaseProjectile.h"
#include "CPMagicProjectile.generated.h"

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
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY(EditDefaultsOnly)
	float Damage;
};
