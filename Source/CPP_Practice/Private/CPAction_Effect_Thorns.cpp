// Fill out your copyright notice in the Description page of Project Settings.


#include "CPAction_Effect_Thorns.h"

#include "CPActionComponent.h"
#include "CPAttributeComponent.h"


UCPAction_Effect_Thorns::UCPAction_Effect_Thorns()
{
	Duration = 0.0f;
}

void UCPAction_Effect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	AActor* Owner = GetOwningComponent()->GetOwner();
	if (UCPAttributeComponent* AttribComp = UCPAttributeComponent::GetAttributes(Owner))
	{
		AttribComp->OnHealthChanged.AddDynamic(this, &UCPAction_Effect_Thorns::OnOwnerHealthChanged);
	}
}

void UCPAction_Effect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	
	AActor* Owner = GetOwningComponent()->GetOwner();
	if (UCPAttributeComponent* AttribComp = UCPAttributeComponent::GetAttributes(Owner))
	{
		AttribComp->OnHealthChanged.RemoveDynamic(this, &UCPAction_Effect_Thorns::OnOwnerHealthChanged);
	}
}

void UCPAction_Effect_Thorns::OnOwnerHealthChanged(AActor* InstigatorActor, UCPAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// 只在受到伤害时生效（Delta < 0）
	if (Delta >= 0.0f)
	{
		return;
	}

	AActor* Owner = GetOwningComponent()->GetOwner();
	if (InstigatorActor == nullptr || InstigatorActor == Owner)
	{
		// 避免自伤或无效攻击者
		return;
	}
	
	float ThornsDamage = -Delta * 0.3f; // 这里假设反弹30%，你可以根据需求调整比例

	if (ThornsDamage <= 0)
	{
		return;
	}

	// 获取攻击者的属性组件，给它扣血
	if (UCPAttributeComponent* InstigatorAttrib = UCPAttributeComponent::GetAttributes(InstigatorActor))
	{
		InstigatorAttrib->ApplyHealthChange(Owner, -ThornsDamage);
	}
}

