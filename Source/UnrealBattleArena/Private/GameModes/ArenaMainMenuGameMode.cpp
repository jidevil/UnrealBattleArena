// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ArenaMainMenuGameMode.h"
#include "Player/ArenaPlayerController.h"
#include "UI/Screens/MainMenu/ArenaMainMenu.h"
#include "Blueprint/UserWidget.h"
#include "UI/ArenaUIManager.h"

AArenaMainMenuGameMode::AArenaMainMenuGameMode()
{
	PlayerControllerClass = AArenaPlayerController::StaticClass();
}

void AArenaMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController && UIManager)
	{
		UIManager->PushScreen(MainMenuClass);
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->SetShowMouseCursor(true);
	}
}
