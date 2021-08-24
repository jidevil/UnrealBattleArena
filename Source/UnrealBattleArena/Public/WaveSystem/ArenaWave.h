// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArenaCharacter.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "ArenaWave.generated.h"

USTRUCT(BlueprintType)
struct FBotDescription : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AArenaBotCharacter> BotCharacterClass{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AArenaBotController> BotControllerClass{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterAttributes Attributes;
};

USTRUCT(BlueprintType)
struct FWaveBotDescription
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BotCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnPoint{ 0 };
};

USTRUCT(BlueprintType)
struct FWaveDescription : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoTrigger{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartTime{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnInterval{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWaveBotDescription> BotDescriptions;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALBATTLEARENA_API UArenaWave : public UActorComponent
{
	GENERATED_BODY()

public:	

	UArenaWave();

	bool IsRunning() const;
	void Start();

protected:

	virtual void BeginPlay() override;
	
	void StartWave();
	void SpawnBot();

public:

	FWaveDescription* WaveDescription{ nullptr };

protected:
	
	UPROPERTY()
	class AArenaWaveSystem* WaveSystem{ nullptr };

	int32 CurrentBot{ 0 };
	int32 CurrentBotCount{ 0 };
	FTimerHandle BotSpawnTimer;
	bool bStarted{ false };
};
