// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPP_PRACTICE_API UCPInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	void PrimaryInteract();
	
	UCPInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	UPROPERTY()
	AActor* FocusedActor;

	// 蓝图中选择要显示的交互提示 WBP
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> DefaultInteractionWidgetClass;

	// 运行时实例，不要暴露在编辑器中
	UPROPERTY()
	UUserWidget* InteractionWidgetInstance;
	
};
