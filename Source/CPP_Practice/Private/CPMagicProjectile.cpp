// Fill out your copyright notice in the Description page of Project Settings.


#include "CPMagicProjectile.h"

#include "CPAttributeComponent.h"
#include "Kismet/GameplayStatics.h"

ACPMagicProjectile::ACPMagicProjectile()
{
	Damage = 20.0f;
}

void ACPMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	
	if (CameraShake)
	{
		UGameplayStatics::PlayWorldCameraShake(
			this,
			CameraShake,
			GetActorLocation(),      // 震动来源位置
			0.f,                     // 最小距离（震动影响范围）
			1000.f                  // 最大距离
		);
	}

	if (OtherActor && OtherActor != GetInstigator())
	{
		if (UCPAttributeComponent* AttributeComp = Cast<UCPAttributeComponent>(
			OtherActor->GetComponentByClass(UCPAttributeComponent::StaticClass())))
		{
			AttributeComp->ApplyHealthChange(-Damage);
		}
	}
}
