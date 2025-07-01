// Fill out your copyright notice in the Description page of Project Settings.


#include "CPCharacter.h"

#include "CPActionComponent.h"
#include "CPAttributeComponent.h"
#include "CPInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACPCharacter::ACPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UCPInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<UCPAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<UCPActionComponent>("ActionComp");
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;
}

void ACPCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ACPCharacter::OnHealthChanged);
}

void ACPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACPCharacter::Jump);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ACPCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("ESkill", IE_Pressed, this, &ACPCharacter::ESkill);
	PlayerInputComponent->BindAction("QSkill", IE_Pressed, this, &ACPCharacter::QSkill);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACPCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACPCharacter::SprintStop);
	
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ACPCharacter::PrimaryInteract);
}

void ACPCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

void ACPCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ACPCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
 	
	AddMovementInput(RightVector, Value);
}

void ACPCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ACPCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void ACPCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ACPCharacter::ESkill()
{
	ActionComp->StartActionByName(this, "Blackhole");
}

void ACPCharacter::QSkill()
{
	ActionComp->StartActionByName(this, "Teleport");
}

void ACPCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ACPCharacter::Jump()
{
	Super::Jump();
}

void ACPCharacter::OnHealthChanged(AActor* InstigatorActor, UCPAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);

		if (NewHealth <= 0.0f)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			DisableInput(PC);
		}
	}
}

FVector ACPCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}


