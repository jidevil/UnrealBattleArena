// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "ArenaWaveSystem.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaWaveSystem : public AActor
{
	GENERATED_BODY()
	
public:	

	void StartWaves();
	void SpawnBot(const FName& BotName, int32 SpawnPointIndex);
	
protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Wave System")
	UDataTable* BotDescriptions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Wave System")
	UDataTable* WaveDescriptions;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Arena Wave System")
	TArray<class AArenaPlayerStart*> SpawnPoints;

	UPROPERTY()
	TArray<class UArenaWave*> Waves;
};
