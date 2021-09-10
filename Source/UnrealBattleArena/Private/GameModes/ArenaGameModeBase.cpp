// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ArenaGameModeBase.h"
#include "Online/ArenaGameState.h"
#include "Kismet/GameplayStatics.h"

AArenaGameModeBase::AArenaGameModeBase()
{
	GameStateClass = AArenaGameState::StaticClass();
}

void AArenaGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}
