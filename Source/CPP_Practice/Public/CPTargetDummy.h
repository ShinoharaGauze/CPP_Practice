// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPTargetDummy.generated.h"

class UCPAttributeComponent;

UCLASS()
class CPP_PRACTICE_API ACPTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPTargetDummy();

protected:

	UPROPERTY(VisibleAnywhere)
	UCPAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCPAttributeComponent* OwningComp, float NewHealth, float Delta);
};
