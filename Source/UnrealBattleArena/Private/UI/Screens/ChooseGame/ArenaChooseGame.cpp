// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Screens/ChooseGame/ArenaChooseGame.h"
#include "UI/Screens/ChooseGame/Widgets/ArenaChooseGameItem.h"
#include "UI/Screens/CreateGame/ArenaCreateGame.h"
#include "UI/ArenaUIManager.h"
#include "Online/ArenaSessionSubsystem.h"
#include "Misc/ArenaFunctionLibrary.h"
#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

void UArenaChooseGame::NativeConstruct()
{
	Super::NativeConstruct();

	if (CreateButton)
	{
		CreateButton->OnClicked.AddDynamic(this, &UArenaChooseGame::OnCreateButtonClicked);
	}

	if (RefreshButton)
	{
		RefreshButton->OnClicked.AddDynamic(this, &UArenaChooseGame::OnRefreshButtonClicked);
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UArenaChooseGame::OnBackButtonClicked);
	}
}

void UArenaChooseGame::NativeDestruct()
{
	Super::NativeDestruct();

	if (CreateButton)
	{
		CreateButton->OnClicked.RemoveDynamic(this, &UArenaChooseGame::OnCreateButtonClicked);
	}

	if (RefreshButton)
	{
		RefreshButton->OnClicked.RemoveDynamic(this, &UArenaChooseGame::OnRefreshButtonClicked);
	}

	if (BackButton)
	{
		BackButton->OnClicked.RemoveDynamic(this, &UArenaChooseGame::OnBackButtonClicked);
	}
}

void UArenaChooseGame::OnFindSessionsCompleted(const TArray<FOnlineSessionSearchResult>& SessionResults,
                                               bool bSuccessful)
{
	UArenaSessionSubsystem* SessionSubsystem = UArenaFunctionLibrary::GetSessionSubsystem(GetWorld());
	if (SessionSubsystem)
	{
		SessionSubsystem->FindSessionCompleteDelegate.RemoveAll(this);
	}

	if (GameItemListBox)
	{
		GameItemListBox->ClearChildren();

		for (const FOnlineSessionSearchResult& SearchResult : SessionResults)
		{
			if (SearchResult.IsValid())
			{
				UArenaChooseGameItem* GameItem = CreateWidget<UArenaChooseGameItem>(
					GetOwningPlayer(), GameItemClass);

				GameItem->SetSessionSearchResult(SearchResult);
				GameItemListBox->AddChild(GameItem);
			}
		}
	}
}

void UArenaChooseGame::OnCreateButtonClicked()
{
	UArenaUIManager* UIManager = UArenaFunctionLibrary::GetUIManager(GetWorld());
	if (UIManager)
	{
		UIManager->PushScreen(CreateGameClass);
	}
}

void UArenaChooseGame::OnRefreshButtonClicked()
{
	UArenaSessionSubsystem* SessionSubsystem = UArenaFunctionLibrary::GetSessionSubsystem(GetWorld());
	if (SessionSubsystem)
	{
		SessionSubsystem->FindSessionCompleteDelegate.AddUObject(this, &UArenaChooseGame::OnFindSessionsCompleted);
		SessionSubsystem->FindSessions(100, false);
	}
}

void UArenaChooseGame::OnBackButtonClicked()
{
	UArenaUIManager* UIManager = UArenaFunctionLibrary::GetUIManager(GetWorld());
	if (UIManager)
	{
		UIManager->PopScreen();
	}
}
