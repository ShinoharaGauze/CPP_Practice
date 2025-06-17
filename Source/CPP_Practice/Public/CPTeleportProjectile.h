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
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
						 UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void Explode();

	void TeleportInstigator();

	FTimerHandle TimerHandle_Explode;
	FTimerHandle TimerHandle_Teleport;

	bool bExploded = false;

	FVector ImpactPoint;

	TWeakObjectPtr<APawn> InstigatorPawn;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	UParticleSystem* ExplosionVFX;

};
