// Fill out your copyright notice in the Description page of Project Settings.


#include "CPHealthPotion.h"

#include "CPAttributeComponent.h"

ACPHealthPotion::ACPHealthPotion()
{
	// 初始状态为激活
	ACPHealthPotion::SetActiveState(true);
}

void ACPHealthPotion::SetActiveState(bool bNewActive)
{
	bIsActive = bNewActive;

	// 控制 Mesh 显隐
	BaseMesh->SetVisibility(bNewActive, true);
	BaseMesh->SetCollisionEnabled(bNewActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}

void ACPHealthPotion::Respawn()
{
	SetActiveState(true);
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
		AttributeComp->ApplyHealthChange(+HealAmount);

		// 进入冷却状态
		SetActiveState(false);

		// 启动定时器，10 秒后重新激活
		GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ACPHealthPotion::Respawn, CooldownTime, false, CooldownTime);
	}
}
