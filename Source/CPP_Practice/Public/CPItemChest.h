// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPickUpBase.h"
#include "CPItemChest.generated.h"

UCLASS()
class CPP_PRACTICE_API ACPItemChest : public ACPPickUpBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float TargetPitch;

	// 这里覆盖基类的实现
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual void UpdateVisualState() override;

	// 构造和生命周期函数
	ACPItemChest();

protected:

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened")
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;
	
};
