// Fill out your copyright notice in the Description page of Project Settings.


#include "CPAnimInstance.h"

#include "CPActionComponent.h"
#include "GameplayTagContainer.h"

void UCPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		ActionComp = Cast<UCPActionComponent>(OwningActor->GetComponentByClass(UCPActionComponent::StaticClass()));
	}
}

void UCPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
