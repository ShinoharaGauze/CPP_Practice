// Fill out your copyright notice in the Description page of Project Settings.


#include "CPCharacter.h"

#include "CPInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ACPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
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

	
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ACPCharacter::PrimaryInteract);
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

void ACPCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ACPCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ACPCharacter::ESkill()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ACPCharacter::ESkill_TimeElapsed, 0.2f);
}

void ACPCharacter::QSkill()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ACPCharacter::QSkill_TimeElapsed, 0.2f);
}

void ACPCharacter::PrimaryAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FVector CameraLocation = CameraComp->GetComponentLocation();
	
	FVector TraceDirection = GetControlRotation().Vector();
	FVector TraceEnd = CameraLocation + TraceDirection * 10000.f;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); 
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, QueryParams);

	FVector TargetPoint;
	if (bHit)
	{
		TargetPoint = HitResult.ImpactPoint;
	}
	else
	{
		TargetPoint = TraceEnd; // 没击中则用远点
	}

	FVector ShotDirection = (TargetPoint - HandLocation).GetSafeNormal();
	FRotator ShotRotation = ShotDirection.Rotation();
	
	FTransform SpawnTM = FTransform(ShotRotation, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(AttackProjectileClass, SpawnTM, SpawnParams);
}

void ACPCharacter::ESkill_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FVector CameraLocation = CameraComp->GetComponentLocation();
	
	FVector TraceDirection = GetControlRotation().Vector();
	FVector TraceEnd = CameraLocation + TraceDirection * 10000.f;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); 
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, QueryParams);

	FVector TargetPoint;
	if (bHit)
	{
		TargetPoint = HitResult.ImpactPoint;
	}
	else
	{
		TargetPoint = TraceEnd; // 没击中则用远点
	}

	FVector ShotDirection = (TargetPoint - HandLocation).GetSafeNormal();
	FRotator ShotRotation = ShotDirection.Rotation();
	
	FTransform SpawnTM = FTransform(ShotRotation, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(ESkillProjectileClass, SpawnTM, SpawnParams);
}

void ACPCharacter::QSkill_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FVector CameraLocation = CameraComp->GetComponentLocation();
	
	FVector TraceDirection = GetControlRotation().Vector();
	FVector TraceEnd = CameraLocation + TraceDirection * 10000.f;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); 
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, QueryParams);

	FVector TargetPoint;
	if (bHit)
	{
		TargetPoint = HitResult.ImpactPoint;
	}
	else
	{
		TargetPoint = TraceEnd; // 没击中则用远点
	}

	FVector ShotDirection = (TargetPoint - HandLocation).GetSafeNormal();
	FRotator ShotRotation = ShotDirection.Rotation();
	
	FTransform SpawnTM = FTransform(ShotRotation, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(QSkillProjectileClass, SpawnTM, SpawnParams);
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
