// Fill out your copyright notice in the Description page of Project Settings.


#include "CPTeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ACPTeleportProjectile::ACPTeleportProjectile()
{
	SphereComp->OnComponentHit.AddDynamic(this, &ACPTeleportProjectile::OnProjectileHit);

	MovementComp->InitialSpeed = 3000.0f;
}

void ACPTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* InstigatorActor = GetInstigator())
	{
		SphereComp->IgnoreActorWhenMoving(InstigatorActor, true);
	}
	
	InstigatorPawn = GetInstigator();
	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ACPTeleportProjectile::Explode, 0.2f);
}

void ACPTeleportProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
											UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bExploded)
	{
		Explode();
	}
}

void ACPTeleportProjectile::Explode()
{
	if (bExploded)
		return;

	bExploded = true;

	ImpactPoint = GetActorLocation();
	
	if (ExplosionVFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionVFX, ImpactPoint);
	}

	if (EffectComp)
	{
		EffectComp->DeactivateSystem();
	}
	
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ACPTeleportProjectile::TeleportInstigator, 0.2f);
	
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	MovementComp->StopMovementImmediately();
}

void ACPTeleportProjectile::TeleportInstigator()
{
	if (InstigatorPawn.IsValid())
	{
		InstigatorPawn->SetActorLocation(ImpactPoint);
	}

	Destroy();
}
