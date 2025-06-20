// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPCharacter.generated.h"


class UCPAttributeComponent;
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
	TSubclassOf<AActor>  QSkillProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;
	
public:
	// Sets default values for this character's properties
	ACPCharacter();

protected:

	UPROPERTY(visibleAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(visibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(visibleAnywhere)
	TObjectPtr<UCPInteractionComponent> InteractionComp;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCPAttributeComponent> AttributeComp;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> HandVfx;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void PrimaryAttack();

	void ESkill();

	void QSkill();
	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

	void PrimaryAttack_TimeElapsed();

	void ESkill_TimeElapsed();

	void QSkill_TimeElapsed();
	
	void PrimaryInteract();

	virtual void Jump() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCPAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

public:	
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
