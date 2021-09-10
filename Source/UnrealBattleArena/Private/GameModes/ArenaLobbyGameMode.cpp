// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ArenaLobbyGameMode.h"
#include "Online/ArenaGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/ArenaFunctionLibrary.h"

void AArenaLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	const FString GameName = UGameplayStatics::ParseOption(OptionsString, "GameName");
	const FString MapName = UGameplayStatics::ParseOption(OptionsString, "MapName");
	const FString TotalPlayers = UGameplayStatics::ParseOption(OptionsString, "TotalPlayers");

	AArenaGameState* ArenaGameState = UArenaFunctionLibrary::GetGameState(GetWorld());
	if (ArenaGameState)
	{
		ArenaGameState->CreateMatch(GameName, MapName, FCString::Atoi(*TotalPlayers));
	}
}
