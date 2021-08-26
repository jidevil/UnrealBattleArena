// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ArenaGameModeBase.h"
#include "UI/ArenaUIManager.h"

void AArenaGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UIManager = NewObject<UArenaUIManager>(this);
	UIManager->RegisterComponent();
}
