// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSystem/ArenaWave.h"
#include "WaveSystem/ArenaWaveSystem.h"

UArenaWave::UArenaWave()
{
}

bool UArenaWave::IsRunning() const
{
	return bStarted;
}

void UArenaWave::Start()
{
	if (bStarted)
	{
		return;
	}

	FTimerHandle WaveTimer;
	GetWorld()->GetTimerManager().SetTimer(WaveTimer, this, &UArenaWave::StartWave,
		WaveDescription->StartTime, false);
}

void UArenaWave::BeginPlay()
{
	Super::BeginPlay();
	WaveSystem = Cast<AArenaWaveSystem>(GetOwner());
}

void UArenaWave::StartWave()
{
	bStarted = true;
	GetWorld()->GetTimerManager().SetTimer(BotSpawnTimer, this, &UArenaWave::SpawnBot,
		WaveDescription->SpawnInterval, true);
}

void UArenaWave::SpawnBot()
{
	const FWaveBotDescription& BotDescription = WaveDescription->BotDescriptions[CurrentBot];

	WaveSystem->SpawnBot(BotDescription.BotName, BotDescription.SpawnPoint);
	CurrentBotCount++;

	if (CurrentBotCount >= BotDescription.BotCount)
	{
		CurrentBotCount = 0;
		CurrentBot++;
	}

	if (CurrentBot >= WaveDescription->BotDescriptions.Num())
	{
		bStarted = false;
		CurrentBot = 0;
		CurrentBotCount = 0;
		GetWorld()->GetTimerManager().ClearTimer(BotSpawnTimer);
	}
}
