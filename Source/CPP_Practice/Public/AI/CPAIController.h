// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "CPAIController.generated.h"

/**
 * 
 */
UCLASS()
class CPP_PRACTICE_API ACPAIController : public AAIController
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;
	
	virtual void BeginPlay() override;
	
};
