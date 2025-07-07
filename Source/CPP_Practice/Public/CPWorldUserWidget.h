// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPWorldUserWidget.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class CPP_PRACTICE_API UCPWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;
	
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn)) 
	AActor* AttachedActor;

protected:

	UPROPERTY(META = (BindWidget))
	USizeBox* ParentSizeBox;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
