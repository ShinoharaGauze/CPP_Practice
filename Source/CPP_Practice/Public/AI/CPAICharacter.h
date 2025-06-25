// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPAICharacter.generated.h"

class UWidgetComponent;
class UCPAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class CPP_PRACTICE_API ACPAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ACPAICharacter();

protected:
	
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCPAttributeComponent> AttributeComp;
	
	virtual void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCPAttributeComponent* OwningComp, float NewHealth, float Delta);

};
