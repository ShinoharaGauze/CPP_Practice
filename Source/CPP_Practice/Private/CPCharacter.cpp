// Fill out your copyright notice in the Description page of Project Settings.


#include "CPCharacter.h"

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
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;
}

void ACPCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ACPCharacter::OnHealthChanged);
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
	SpawnProjectile(AttackProjectileClass);
}

void ACPCharacter::ESkill_TimeElapsed()
{
	SpawnProjectile(ESkillProjectileClass);
}

void ACPCharacter::QSkill_TimeElapsed()
{
	SpawnProjectile(QSkillProjectileClass);
}

void ACPCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = CameraComp->GetComponentLocation();

		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);

		FHitResult Hit;

		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}

	if (HandVfx)
	{
		UGameplayStatics::SpawnEmitterAttached(
			HandVfx,
			GetMesh(),                          // 父组件（角色的 SkeletalMesh）
			TEXT("Muzzle_01"),                  // 挂载点 Socket 名（确保存在）
			FVector::ZeroVector,                // 相对位置
			FRotator::ZeroRotator,              // 相对旋转
			EAttachLocation::SnapToTarget,      // 对齐方式
			true                                // 自动销毁
		);
	}
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


