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

	virtual float GetBottomOffset() const;
	
	virtual void SetActiveState(bool bIsActive);

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	virtual void Respawn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup")
	bool CanInteract(APawn* InstigatorPawn) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup")
	void Interact(APawn* InstigatorPawn);

protected:
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;
	
	UPROPERTY(ReplicatedUsing="OnRep_IsActive")
	bool bIsActive;

	UFUNCTION()
	void OnRep_IsActive();

	virtual void UpdateVisualState();
};
