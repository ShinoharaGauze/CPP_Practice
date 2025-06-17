// Fill out your copyright notice in the Description page of Project Settings.


#include "CPAttributeComponent.h"

// Sets default values for this component's properties
UCPAttributeComponent::UCPAttributeComponent()
{
	Health = 100;
}

bool UCPAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	return true;
}
