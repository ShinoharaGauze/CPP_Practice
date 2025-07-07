// Fill out your copyright notice in the Description page of Project Settings.


#include "CPBaseProjectile.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ACPBaseProjectile::ACPBaseProjectile()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentHit.AddDynamic(this, &ACPBaseProjectile::OnActorHit);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 8000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;

	ProjectileSoundComp = CreateDefaultSubobject<UAudioComponent>("ProjectileSoundComp");
	ProjectileSoundComp->SetupAttachment(RootComponent);
	ProjectileSoundComp->bAutoActivate = false;
	
	SetReplicates(true);
}

void ACPBaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* MyInstigator = GetInstigator())
	{
		SphereComp->IgnoreActorWhenMoving(MyInstigator, true);
	}

	if (ProjectileSoundComp)
	{
		ProjectileSoundComp->Play();
	}
}

void ACPBaseProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

// Called when the game starts or when spawned
void ACPBaseProjectile::Explode_Implementation()
{
	if (ProjectileSoundComp && ProjectileSoundComp->IsPlaying())
	{
		ProjectileSoundComp->Stop();
	}

	if (ExplodeSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());
	}
	
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

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

	Destroy();
}

void ACPBaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
