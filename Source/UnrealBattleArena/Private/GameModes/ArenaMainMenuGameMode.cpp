// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ArenaMainMenuGameMode.h"
#include "Player/ArenaPlayerController.h"

AArenaMainMenuGameMode::AArenaMainMenuGameMode()
{
	PlayerControllerClass = AArenaPlayerController::StaticClass();
}

void AArenaMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
}
