// Fill out your copyright notice in the Description page of Project Settings.


#include "CPItemChest.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ACPItemChest::ACPItemChest()
{
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.0f;

    bLidOpened = false;
}

void ACPItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!HasAuthority())
	{
		return;
	}

	bLidOpened = !bLidOpened;

	UpdateVisualState();
}

void ACPItemChest::OnActorLoaded_Implementation()
{
	UpdateVisualState();
}

void ACPItemChest::UpdateVisualState()
{
	float CurrPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}

void ACPItemChest::OnRep_LidOpened()
{
	UpdateVisualState();
}

void ACPItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPItemChest, bLidOpened);
}


