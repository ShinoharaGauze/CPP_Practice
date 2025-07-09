// Fill out your copyright notice in the Description page of Project Settings.


#include "CPCoin.h"

#include "CPPlayerState.h"

ACPCoin::ACPCoin()
{
	ACPCoin::SetActiveState(true); // 初始为可交互
}

void ACPCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!HasAuthority() || !bIsActive)
	{
		return;
	}

	// 尝试给 PlayerState 增加 Credits
	if (APlayerState* PlayerState = InstigatorPawn->GetPlayerState())
	{
		if (ACPPlayerState* CPPlayerState = Cast<ACPPlayerState>(PlayerState))
		{
			CPPlayerState->AddCredits(CreditAmount);
		}
	}

	// 隐藏并进入冷却
	SetActiveState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ACPCoin::Respawn, CooldownTime, false);
}