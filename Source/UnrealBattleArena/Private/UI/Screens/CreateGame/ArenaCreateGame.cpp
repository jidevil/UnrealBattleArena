// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Screens/CreateGame/ArenaCreateGame.h"
#include "UI/Widgets/ArenaSelectList.h"
#include "UI/ArenaUIManager.h"
#include "Online/ArenaSessionSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/ArenaFunctionLibrary.h"
#include "Player/ArenaPlayerController.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "OnlineSessionSettings.h"

void UArenaCreateGame::NativeConstruct()
{
	Super::NativeConstruct();

	if (MapNameSelectList)
	{
		const TArray<FString> Maps =
		{
			"Map_Level01"
		};
		
		for (const FString& Map : Maps)
		{
			MapNameSelectList->AddItem(FText::FromString(Map), Map);
		}

		if (Maps.Num() > 0)
		{
			MapNameSelectList->SetCurrentItem(0);
		}
	}

	if (PlayersSelectList)
	{
		constexpr int32 MaxPlayers = 5;
		
		for (int32 Index = 1; Index <= MaxPlayers; Index++)
		{
			PlayersSelectList->AddItem(FText::AsNumber(Index),
				FString::FromInt(Index));
		}

		if (MaxPlayers > 0)
		{
			PlayersSelectList->SetCurrentItem(0);
		}
	}

	if (CreateButton)
	{
		CreateButton->OnClicked.AddDynamic(this, &UArenaCreateGame::OnCreateButtonClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UArenaCreateGame::OnCancelButtonClicked);
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UArenaCreateGame::OnBackButtonClicked);
	}
}

void UArenaCreateGame::NativeDestruct()
{
	Super::NativeDestruct();

	if (CreateButton)
	{
		CreateButton->OnClicked.RemoveDynamic(this, &UArenaCreateGame::OnCreateButtonClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.RemoveDynamic(this, &UArenaCreateGame::OnCancelButtonClicked);
	}

	if (BackButton)
	{
		BackButton->OnClicked.RemoveDynamic(this, &UArenaCreateGame::OnBackButtonClicked);
	}
}

FString UArenaCreateGame::GetOptionsString()
{
	const FSelectItem& PlayersItem = PlayersSelectList->GetSelectedItem();
	const FSelectItem& MapItem = MapNameSelectList->GetSelectedItem();
	
	const FString Options = FString::Printf(TEXT("%s?GameName=%s?MapName=%s?PlayerName=%s?TotalPlayers=%s"),
		TEXT("listen"),	*GameNameEditText->Text.ToString(), *MapItem.Value,
		TEXT("ArenaPlayer"), *PlayersItem.Value);

	return Options;
}

void UArenaCreateGame::OnCreateSessionCompleted(bool bSuccessful)
{
	UArenaSessionSubsystem* SessionSubsystem = UArenaFunctionLibrary::GetSessionSubsystem(GetWorld());
	if (SessionSubsystem)
	{
		SessionSubsystem->CreateSessionCompleteDelegate.RemoveDynamic(this,
			&UArenaCreateGame::OnCreateSessionCompleted);

		AArenaPlayerController* PlayerController = UArenaFunctionLibrary::GetPlayerController(GetWorld());
		if (PlayerController)
		{
			const FName LobbyLevel{ "Map_Lobby" };
			UGameplayStatics::OpenLevel(GetWorld(), LobbyLevel, true, GetOptionsString());
		}
	}
}

void UArenaCreateGame::OnCreateButtonClicked()
{
	UArenaSessionSubsystem* SessionSubsystem = UArenaFunctionLibrary::GetSessionSubsystem(GetWorld());
	if (SessionSubsystem)
	{
		SessionSubsystem->CreateSessionCompleteDelegate.AddDynamic(this,
			&UArenaCreateGame::OnCreateSessionCompleted);
		
		const FSelectItem& SelectedMapItem = MapNameSelectList->GetSelectedItem();
		const FSelectItem& SelectedPlayers = PlayersSelectList->GetSelectedItem();
		
		TMap<FName, FString> Settings;
		Settings.Add(SETTING_GAMENAME, GameNameEditText->GetText().ToString());
		Settings.Add(SETTING_CREATEDBY, "PlayerName");
		Settings.Add(SETTING_PLAYERNAME, "PlayerName");
		Settings.Add(SETTING_MAPNAME, SelectedMapItem.Value);
		Settings.Add(SETTING_TOTALPLAYERS, SelectedPlayers.Value);
		
		SessionSubsystem->CreateSession(NumPublicConnections, bIsLANMatch, Settings);
	}
}

void UArenaCreateGame::OnCancelButtonClicked()
{
	UArenaUIManager* UIManager = UArenaFunctionLibrary::GetUIManager(GetWorld());
	if (UIManager)
	{
		UIManager->PopScreen();
	}
}

void UArenaCreateGame::OnBackButtonClicked()
{
	UArenaUIManager* UIManager = UArenaFunctionLibrary::GetUIManager(GetWorld());
	if (UIManager)
	{
		UIManager->PopScreen();
	}
}
