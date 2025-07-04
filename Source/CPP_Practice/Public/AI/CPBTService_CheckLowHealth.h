// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CPBTService_CheckLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class CPP_PRACTICE_API UCPBTService_CheckLowHealth : public UBTService
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector LowHealthKey;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float LowHealthFraction = 0.3f;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
