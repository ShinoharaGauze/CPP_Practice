// Fill out your copyright notice in the Description page of Project Settings.


#include "CPAttributeComponent.h"

#include "CPGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable CVarDamageMultiplier(TEXT("cp.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

UCPAttributeComponent::UCPAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
	RageMax = 100;
	Rage = 0.0f;

	SetIsReplicatedByDefault(true);
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

float UCPAttributeComponent::GetRage() const
{
	return Rage;
}

float UCPAttributeComponent::GetRageMax() const
{
	return RageMax;
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

void UCPAttributeComponent::SetRageMax(float NewMax, bool bUpdateCurrent)
{
	if (NewMax <= 0.f)
	{
		return;
	}

	RageMax = NewMax;

	if (bUpdateCurrent)
	{
		Rage = RageMax;
	}
	else
	{
		Rage = FMath::Clamp(Rage, 0.f, RageMax);
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

	float RageCoefficient = 0.3f;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	const float ActualDelta = Health - OldHealth;
	//OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	if (ActualDelta != 0.0f)
	{
		MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
	}

	if (ActualDelta < 0.0f) // 只有受到伤害才加怒气
	{
		float DeltaRage = RageCoefficient * FMath::Abs(ActualDelta);
		ApplyRageChange(DeltaRage);
	}

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

bool UCPAttributeComponent::ApplyRageChange(float Delta)
{
	const float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

	const float ActualDelta = Rage - OldRage;
	OnRageChanged.Broadcast(this, Rage, ActualDelta);

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

void UCPAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void UCPAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCPAttributeComponent, Health);
	DOREPLIFETIME(UCPAttributeComponent, HealthMax);

	//DOREPLIFETIME_CONDITION(UCPAttributeComponent, HealthMax, COND_OwnerOnly);
}
