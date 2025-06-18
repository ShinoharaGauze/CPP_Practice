// Fill out your copyright notice in the Description page of Project Settings.


#include "CPTargetDummy.h"

#include "CPAttributeComponent.h"

// Sets default values
ACPTargetDummy::ACPTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UCPAttributeComponent>("AttributeComp");
	AttributeComp->OnHealthChanged.AddDynamic(this, &ACPTargetDummy::OnHealthChanged);

}

void ACPTargetDummy::OnHealthChanged(AActor* InstigatorActor, UCPAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta <= 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}


