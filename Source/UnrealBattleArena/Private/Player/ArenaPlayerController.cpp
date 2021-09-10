// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ArenaPlayerController.h"
#include "Character/ArenaCharacter.h"
#include "UI/HUD/ArenaHUD.h"
#include "UI/Screens/MainMenu/ArenaMainMenu.h"
#include "UI/Screens/Lobby/ArenaLobby.h"
#include "Blueprint/UserWidget.h"
#include "GameModes/ArenaGameMode.h"
#include "GameModes/ArenaGameModeBase.h"
#include "GameModes/ArenaLobbyGameMode.h"
#include "GameModes/ArenaMainMenuGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/ArenaFunctionLibrary.h"
#include "Online/ArenaGameState.h"
#include "UI/ArenaUIManager.h"

void AArenaPlayerController::ShowMainMenu()
{
	if (IsLocalPlayerController())
	{
		UIManager->PushScreen(MainMenuClass);
	}
}

void AArenaPlayerController::ShowLobby()
{
	if (IsLocalPlayerController())
	{
		UIManager->PushScreen(LobbyClass);
	}
}

void AArenaPlayerController::ShowHUD()
{
	if (IsLocalPlayerController())
	{
		PlayerHUD = Cast<UArenaHUD>(CreateWidget(this, PlayerHUDClass));
		PlayerHUD->AddToViewport();
	}
}

void AArenaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalPlayerController())
	{
		UIManager = NewObject<UArenaUIManager>(this, UIManagerClass);
		UIManager->RegisterComponent();
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		FTimerDelegate DelayCallback;
		DelayCallback.BindLambda([this]
		{
			AArenaGameModeBase* GameMode = UArenaFunctionLibrary::GetGameMode(GetWorld());
			if (GameMode)
			{
				if (GameMode->IsA(AArenaMainMenuGameMode::StaticClass()))
				{
					ClientOnMainMenu();
				}
				else if (GameMode->IsA(AArenaLobbyGameMode::StaticClass()))
				{
					ClientOnLobby();
				}
				else if (GameMode->IsA(AArenaGameMode::StaticClass()))
				{
					ClientOnGameplay();
				}
			}	
		});

		FTimerHandle DelayTimer;
		GetWorldTimerManager().SetTimer(DelayTimer, DelayCallback, 1.0f, false);
	}
}

void AArenaPlayerController::ServerJoinMatch_Implementation()
{
	AArenaGameState* GameState = UArenaFunctionLibrary::GetGameState(GetWorld());
	if (GameState)
	{
		GameState->JoinMatch("PlayerName");
	}
}

void AArenaPlayerController::ClientOnMainMenu_Implementation()
{
	if (IsLocalPlayerController())
	{
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
	}
	
	ShowMainMenu();
}

void AArenaPlayerController::ClientOnLobby_Implementation()
{
	if (IsLocalPlayerController())
	{
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
	}

	ShowLobby();
	ServerJoinMatch();
}

void AArenaPlayerController::ClientOnGameplay_Implementation()
{
	if (IsLocalPlayerController())
	{
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
	}
	
	ShowHUD();
}