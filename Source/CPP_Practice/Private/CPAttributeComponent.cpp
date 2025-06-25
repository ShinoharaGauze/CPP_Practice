// Fill out your copyright notice in the Description page of Project Settings.


#include "CPAttributeComponent.h"

// Sets default values for this component's properties
UCPAttributeComponent::UCPAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
}

bool UCPAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

float UCPAttributeComponent::GetHealth() const
{
	return Health;
}

float UCPAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

void UCPAttributeComponent::SetHealthMax(float NewMax, bool bUpdateCurrent)
{
	if (NewMax <= 0.f)
	{
		return;
	}

	HealthMax = NewMax;

	if (bUpdateCurrent)
	{
		Health = HealthMax;
	}
	else
	{
		Health = FMath::Clamp(Health, 0.f, HealthMax);
	}
}

bool UCPAttributeComponent::ApplyHealthChange(float Delta)
{
	const float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	const float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);
	
	return ActualDelta != 0.0f;
}
