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

bool UCPAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	Health = std::clamp(Health, 0.0f, HealthMax);
	
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true;
}
