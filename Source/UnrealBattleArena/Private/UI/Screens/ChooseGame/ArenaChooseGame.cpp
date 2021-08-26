// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Screens/ChooseGame/ArenaChooseGame.h"
#include "UI/Screens/ChooseGame/Widgets/ArenaGameItem.h"
#include "Online/ArenaSessionSubsystem.h"
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

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (GameInstance)
	{
		UArenaSessionSubsystem* SessionSubsystem = GameInstance->GetSubsystem<UArenaSessionSubsystem>();
		if (SessionSubsystem)
		{
			SessionSubsystem->FindSessionCompleteDelegate.AddUObject(this, &UArenaChooseGame::OnFindSessionsCompleted);
		}
	}
}

void UArenaChooseGame::OnFindSessionsCompleted(const TArray<FOnlineSessionSearchResult>& SessionResults,
	bool bSuccessful)
{
	if (GameItemListBox)
	{
		GameItemListBox->ClearChildren();
	}
	
	if (bSuccessful)
	{
		for (const FOnlineSessionSearchResult& SearchResult : SessionResults)
		{
			if (SearchResult.IsValid())
			{
				UArenaGameItem* GameItem = Cast<UArenaGameItem>(CreateWidget(this,
					GameItemClass));

				if (GameItem)
				{
					GameItem->SetSessionId(SearchResult.GetSessionIdStr());
					GameItem->SetPadding(FMargin(0.0f, 10.0f));
					GameItemListBox->AddChild(GameItem);
				}
			}
		}
	}
}

void UArenaChooseGame::OnCreateButtonClicked()
{
}

void UArenaChooseGame::OnRefreshButtonClicked()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (GameInstance)
	{
		UArenaSessionSubsystem* SessionSubsystem = GameInstance->GetSubsystem<UArenaSessionSubsystem>();
		if (SessionSubsystem)
		{
			SessionSubsystem->FindSessions(20, false);
		}
	}
}
