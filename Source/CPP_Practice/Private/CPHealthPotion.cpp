// Fill out your copyright notice in the Description page of Project Settings.


#include "CPHealthPotion.h"

#include "CPAttributeComponent.h"
#include "CPPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ACPHealthPotion::ACPHealthPotion()
{
	// 初始状态为激活
	ACPHealthPotion::SetActiveState(true);
}

FText ACPHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	UCPAttributeComponent* AttributeComp = UCPAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp && AttributeComp->GetHealth() == AttributeComp->GetHealthMax())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full Health");
	}

	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores {1} HP. "), CreditCost, HealAmount);
}

void ACPHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!HasAuthority() || !bIsActive)
	{
		return;
	}

	// 获取属性组件
	if (UCPAttributeComponent* AttributeComp = Cast<UCPAttributeComponent>(InstigatorPawn->GetComponentByClass(UCPAttributeComponent::StaticClass())))
	{
		// 获取玩家状态
		if (APlayerState* PlayerState = InstigatorPawn->GetPlayerState())
		{
			if (ACPPlayerState* CPPlayerState = Cast<ACPPlayerState>(PlayerState))
			{
				// 血满或积分不足都不治疗
				if (AttributeComp->GetHealth() >= AttributeComp->GetHealthMax() ||
					CPPlayerState->GetCredits() < CreditCost)
				{
					return;
				}

				// 扣除积分
				CPPlayerState->AddCredits(-CreditCost);
			}
		}

		// 治疗
		AttributeComp->ApplyHealthChange(this, +HealAmount);

		// 进入冷却
		SetActiveState(false);
		GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ACPHealthPotion::Respawn, CooldownTime, false);
	}
}

#undef LOCTEXT_NAMESPACE
