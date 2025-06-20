// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPickUpBase.h"

// Sets default values
ACPPickUpBase::ACPPickUpBase()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;
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
}

