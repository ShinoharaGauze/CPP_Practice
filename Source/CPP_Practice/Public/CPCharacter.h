// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPCharacter.generated.h"


class UCPAction;
class UCPActionComponent;
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

	UPROPERTY(visibleAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(visibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(visibleAnywhere)
	TObjectPtr<UCPInteractionComponent> InteractionComp;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCPAttributeComponent> AttributeComp;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCPActionComponent> ActionComp;

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	TArray<TSubclassOf<UCPAction>> MovementBlockingActions;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void SprintStart();

	void SprintStop();

	void PrimaryAttack();

	void ESkill();

	void QSkill();
	
	void PrimaryInteract();

	virtual void Jump() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCPAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	virtual FVector GetPawnViewLocation() const override;
	
	virtual void PostInitializeComponents() override;

public:	

	ACPCharacter();
	
	bool IsMovementBlocked() const;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);	
};
