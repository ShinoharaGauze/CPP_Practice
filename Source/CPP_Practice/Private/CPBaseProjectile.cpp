// Fill out your copyright notice in the Description page of Project Settings.


#include "CPBaseProjectile.h"

#include "CPAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ACPBaseProjectile::ACPBaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 0;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACPBaseProjectile::OnActorOverlap);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	
}

void ACPBaseProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (UCPAttributeComponent* AttributeComp = Cast<UCPAttributeComponent>(OtherActor->GetComponentByClass(UCPAttributeComponent::StaticClass())))
		{
			AttributeComp->ApplyHealthChange(0);
			
			Destroy();
			
		}
	}
}

// Called when the game starts or when spawned
void ACPBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

