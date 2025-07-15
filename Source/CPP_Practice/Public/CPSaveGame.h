// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CPSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString ActorName;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	TArray<uint8> ByteData;
};

/**
 * 
 */
UCLASS()
class CPP_PRACTICE_API UCPSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	float Credits;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
};
