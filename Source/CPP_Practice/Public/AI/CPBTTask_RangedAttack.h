// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CPBTTask_RangedAttack.generated.h"

class UCPAction_Effect;
/**
 * 
 */
UCLASS()
class CPP_PRACTICE_API UCPBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletSpread;

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Damage")
	TSubclassOf<UCPAction_Effect> BurningActionClass;

	UPROPERTY(EditAnywhere, Category = "Damage")
	TSubclassOf<UCPAction_Effect> StunnedActionClass;

public:

	UCPBTTask_RangedAttack();
};
