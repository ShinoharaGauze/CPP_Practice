// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class CPP_PRACTICE_API ACPAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ACPAICharacter();

protected:

	virtual void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

};
