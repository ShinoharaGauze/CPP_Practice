// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPickUpBase.h"
#include "CPSprintGiver.generated.h"

class UCPAction;
/**
 * 
 */
UCLASS()
class CPP_PRACTICE_API ACPSprintGiver : public ACPPickUpBase
{
	GENERATED_BODY()

public:
	ACPSprintGiver();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual bool CanInteract_Implementation(APawn* InstigatorPawn) const override;

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TSubclassOf<UCPAction> SprintActionClass;
};
