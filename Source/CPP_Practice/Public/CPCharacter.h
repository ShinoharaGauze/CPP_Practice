// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPCharacter.generated.h"


class UCPInteractionComponent;
class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;

UCLASS()
class CPP_PRACTICE_API ACPCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> AttackProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ESkillProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> QSkillProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;
	
public:
	// Sets default values for this character's properties
	ACPCharacter();

protected:

	UPROPERTY(visibleAnywhere, Category = "Component")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(visibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(visibleAnywhere)
	TObjectPtr<UCPInteractionComponent> InteractionComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void PrimaryAttack();

	void ESkill();

	void QSkill();

	void PrimaryAttack_TimeElapsed();

	void ESkill_TimeElapsed();

	void QSkill_TimeElapsed();
	
	void PrimaryInteract();
	
	void Jump();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
