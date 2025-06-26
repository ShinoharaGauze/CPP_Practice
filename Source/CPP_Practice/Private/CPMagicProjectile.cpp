// Fill out your copyright notice in the Description page of Project Settings.


#include "CPMagicProjectile.h"

#include "CPAttributeComponent.h"
#include "Components/SphereComponent.h"

ACPMagicProjectile::ACPMagicProjectile()
{
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACPMagicProjectile::OnActorOverlap);
	
	Damage = 20.0f;
}

void ACPMagicProjectile::OnActorOverlap(::UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		Explode();
			
		if (UCPAttributeComponent* AttributeComp = Cast<UCPAttributeComponent>(OtherActor->GetComponentByClass(UCPAttributeComponent::StaticClass())))
		{
			AttributeComp->ApplyHealthChange(GetInstigator(), -Damage);
		}
	}
}
