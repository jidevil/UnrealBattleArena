// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ArenaCoopGameMode.h"
#include "WaveSystem/ArenaWaveSystem.h"
#include "Kismet/GameplayStatics.h"

int32 AArenaCoopGameMode::ChooseTeam(AArenaPlayerState* PlayerState)
{
	if (PlayerState)
	{
		constexpr int32 PlayerTeamNumber = 0;
		constexpr int32 BotTeamNumber = 1;

		return PlayerState->IsBot() ? BotTeamNumber : PlayerTeamNumber;
	}

	return Super::ChooseTeam(PlayerState);
}

void AArenaCoopGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaWaveSystem::StaticClass(), OutActors);

	if (OutActors.Num() > 0)
	{
		WaveSystem = Cast<AArenaWaveSystem>(OutActors[0]);
	}

	if (WaveSystem)
	{
		WaveSystem->StartWaves();
	}
}
