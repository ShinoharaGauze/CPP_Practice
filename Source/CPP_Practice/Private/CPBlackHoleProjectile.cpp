// Fill out your copyright notice in the Description page of Project Settings.


#include "CPBlackHoleProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACPBlackHoleProjectile::ACPBlackHoleProjectile()
{
	MovementComp->InitialSpeed = 1000.0f;
}
