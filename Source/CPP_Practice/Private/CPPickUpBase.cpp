// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPickUpBase.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ACPPickUpBase::ACPPickUpBase()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	bIsActive = true;

	bReplicates = true;
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

FText ACPPickUpBase::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void ACPPickUpBase::SetActiveState(bool bNewActive)
{
	if (HasAuthority()) // 仅服务端可以设置
	{
		bIsActive = bNewActive;
		// 客户端的视觉由 OnRep 来处理
		UpdateVisualState(); // 手动调用以同步视觉给本地客户端（监听服务器）
	}
}

void ACPPickUpBase::Respawn()
{
	SetActiveState(true);
}

void ACPPickUpBase::OnRep_IsActive()
{
	// 所有客户端都在这里更新表现
	UpdateVisualState();
}

void ACPPickUpBase::UpdateVisualState()
{
	if (BaseMesh)
	{
		BaseMesh->SetVisibility(bIsActive, true);
		BaseMesh->SetCollisionEnabled(bIsActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

void ACPPickUpBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPPickUpBase, bIsActive);
}

