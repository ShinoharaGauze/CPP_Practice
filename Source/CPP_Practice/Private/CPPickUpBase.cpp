// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPickUpBase.h"

// Sets default values
ACPPickUpBase::ACPPickUpBase()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	SetReplicates(true);
}

float ACPPickUpBase::GetBottomOffset() const
{
	if (BaseMesh)
	{
		FVector Origin, BoxExtent;
		BaseMesh->GetLocalBounds(Origin, BoxExtent);
		return Origin.Z - BoxExtent.Z; // 底部相对原点偏移（负数）
	}
	return 0.f;
}

void ACPPickUpBase::Interact_Implementation(APawn* InstigatorPawn)
{
	// 空实现，子类 override 实际行为
}

bool ACPPickUpBase::CanInteract_Implementation(APawn* InstigatorPawn) const
{
	return bIsActive;
}

void ACPPickUpBase::SetActiveState(bool bNewActive)
{
	bIsActive = bNewActive;

	if (BaseMesh)
	{
		BaseMesh->SetVisibility(bNewActive, true);
		BaseMesh->SetCollisionEnabled(bNewActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

void ACPPickUpBase::Respawn()
{
	SetActiveState(true);
}

