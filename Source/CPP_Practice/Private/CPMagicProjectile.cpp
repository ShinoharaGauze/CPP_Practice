// Fill out your copyright notice in the Description page of Project Settings.


#include "CPMagicProjectile.h"

#include "CPAttributeComponent.h"

ACPMagicProjectile::ACPMagicProjectile()
{
	Damage = 20.0f;
}

void ACPMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	if (OtherActor && OtherActor != GetInstigator())
	{
		if (UCPAttributeComponent* AttributeComp = Cast<UCPAttributeComponent>(
			OtherActor->GetComponentByClass(UCPAttributeComponent::StaticClass())))
		{
			AttributeComp->ApplyHealthChange(-Damage);
		}
	}
}
