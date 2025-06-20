// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "CPPickUpBase.generated.h"

UCLASS()
class CPP_PRACTICE_API ACPPickUpBase : public AActor, public ICPGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPPickUpBase();
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual bool CanInteract_Implementation(APawn* InstigatorPawn) const override;

protected:
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;
	
	UPROPERTY()
	bool bIsActive = true;

	virtual void SetActiveState(bool bIsActive);
};
