// Fill out your copyright notice in the Description page of Project Settings.


#include "CPAttributeComponent.h"

#include "CPGameModeBase.h"

static TAutoConsoleVariable CVarDamageMultiplier(TEXT("cp.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

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

bool UCPAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
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

bool UCPAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}
	
	const float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	const float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ACPGameModeBase* GM = GetWorld()->GetAuthGameMode<ACPGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	
	return ActualDelta != 0.0f;
}

UCPAttributeComponent* UCPAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UCPAttributeComponent>(FromActor->GetComponentByClass(UCPAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UCPAttributeComponent::IsActorAlive(AActor* Actor)
{
	UCPAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}
