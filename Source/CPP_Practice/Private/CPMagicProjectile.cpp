// Fill out your copyright notice in the Description page of Project Settings.


#include "CPMagicProjectile.h"

#include "CPActionComponent.h"
#include "CPAction_Effect.h"
#include "CPGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACPMagicProjectile::ACPMagicProjectile()
{
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACPMagicProjectile::OnActorOverlap);
	
	DamageAmount = 20.0f;
}

void ACPMagicProjectile::OnActorOverlap(::UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{ 
		UCPActionComponent* ActionComp = Cast<UCPActionComponent>(OtherActor->GetComponentByClass(UCPActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		
		if (UCPGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();

			if (ActionComp && HasAuthority())
			{
				if (FMath::FRand() < 0.333f)
				{
					ActionComp->AddAction(GetInstigator(), BurningActionClass);
				}
			}
		}
	}
}
