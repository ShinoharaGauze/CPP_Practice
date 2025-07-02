// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPAction_Effect.h"
#include "CPAttributeComponent.h"
#include "CPAction_Effect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class CPP_PRACTICE_API UCPAction_Effect_Thorns : public UCPAction_Effect
{
	GENERATED_BODY()
	
public:

	void StartAction_Implementation(AActor* Instigator) override;
	
	void StopAction_Implementation(AActor* Instigator) override;

protected:

	UFUNCTION()
	void OnOwnerHealthChanged(AActor* InstigatorActor, UCPAttributeComponent* OwningComp, float NewHealth, float Delta);

public:

	UCPAction_Effect_Thorns();
	
};
