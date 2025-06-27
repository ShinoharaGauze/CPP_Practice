// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPAICharacter.generated.h"

class UCPWorldUserWidget;
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
	
	virtual void PostInitializeComponents() override;

	UPROPERTY()
	UCPWorldUserWidget* ActiveHealthBar;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;
		
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCPAttributeComponent> AttributeComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Credits")
	float Credit = 10.0f;

	UFUNCTION()
	void SetTargetActor(AActor* NewTarget);
	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCPAttributeComponent* OwningComp, float NewHealth, float Delta);

public:
	UFUNCTION(BlueprintCallable, Category = "Credits")
	float GetCredit() const { return Credit; }

};
