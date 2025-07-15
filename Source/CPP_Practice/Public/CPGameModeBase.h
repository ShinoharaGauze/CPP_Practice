// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "CPGameModeBase.generated.h"

class UCPSaveGame;
class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;

/**
 * 
 */
UCLASS()
class CPP_PRACTICE_API ACPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	FString SlotName;

	UPROPERTY()
	UCPSaveGame* CurrentSaveGame;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	UEnvQuery* SpawnPickupQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TArray<TSubclassOf<AActor>> PickupClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	int32 MaxPickupSpawnCount;
		
	FTimerHandle TimerHandle_SpawnBots;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	UFUNCTION()
	void OnSpawnPickupQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
public:

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	ACPGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual void StartPlay() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	UFUNCTION(BlueprintCallable, Category = "LoadGame")
	void LoadSaveGame();
};
