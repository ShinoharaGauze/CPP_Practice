// Fill out your copyright notice in the Description page of Project Settings.


#include "CPHealthPotion.h"

#include "CPAttributeComponent.h"
#include "CPPlayerState.h"

ACPHealthPotion::ACPHealthPotion()
{
	// 初始状态为激活
	ACPHealthPotion::SetActiveState(true);
}

bool ACPHealthPotion::CanInteract_Implementation(APawn* InstigatorPawn) const
{
	if (!bIsActive)
	{
		return false;
	}

	if (UCPAttributeComponent* AttributeComp = Cast<UCPAttributeComponent>(InstigatorPawn->GetComponentByClass(UCPAttributeComponent::StaticClass())))
	{
		// 如果满血则不可交互
		return AttributeComp->GetHealth() < AttributeComp->GetHealthMax();
	}

	if (const APlayerState* PlayerState = InstigatorPawn->GetPlayerState())
	{
		if (const ACPPlayerState* CPPlayerState = Cast<ACPPlayerState>(PlayerState))
		{
			return CPPlayerState->GetCredits() >= CreditCost;
		}
	}
	
	return false;
}

void ACPHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!CanInteract_Implementation(InstigatorPawn))
	{
		return;
	}

	if (UCPAttributeComponent* AttributeComp = Cast<UCPAttributeComponent>(InstigatorPawn->GetComponentByClass(UCPAttributeComponent::StaticClass())))
	{
		if (APlayerState* PlayerState = InstigatorPawn->GetPlayerState())
		{
			if (ACPPlayerState* CPPlayerState = Cast<ACPPlayerState>(PlayerState))
			{
				// 再次判断，保险起见
				if (CPPlayerState->GetCredits() >= CreditCost)
				{
					CPPlayerState->AddCredits(-CreditCost);
				}
				else
				{
					// 不足积分，不进行治疗
					return;
				}
			}
		}
		
		AttributeComp->ApplyHealthChange(this, +HealAmount);

		// 进入冷却状态
		SetActiveState(false);

		// 启动定时器，10 秒后重新激活
		GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ACPHealthPotion::Respawn, CooldownTime, false, CooldownTime);
	}
}
