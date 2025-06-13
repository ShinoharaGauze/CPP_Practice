// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPExplosiveBarrel.generated.h"

class URadialForceComponent;

UCLASS()
class CPP_PRACTICE_API ACPExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPExplosiveBarrel();

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URadialForceComponent> RadialForceComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnActorHit(
		UPrimitiveComponent * HitComponent,
		AActor * OtherActor,
		UPrimitiveComponent * OtherComp,
		FVector NormalImpulse,
		const FHitResult & Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
