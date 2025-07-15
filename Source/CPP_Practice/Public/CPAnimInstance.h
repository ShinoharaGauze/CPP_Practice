// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CPAnimInstance.generated.h"

class UCPActionComponent;
/**
 * 
 */
UCLASS()
class CPP_PRACTICE_API UCPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsStunned;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UCPActionComponent* ActionComp;
	
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
