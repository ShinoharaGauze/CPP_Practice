// Fill out your copyright notice in the Description page of Project Settings.


#include "CPItemChest.h"

// Sets default values
ACPItemChest::ACPItemChest()
{
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.0f;
}

void ACPItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));

	SetActiveState(false);
}


