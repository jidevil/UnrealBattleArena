// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Screens/ChooseGame//Widgets/ArenaChooseGameItem.h"
#include "Online/ArenaSessionSubsystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Misc/ArenaFunctionLibrary.h"
#include "Player/ArenaPlayerController.h"

void UArenaChooseGameItem::SetSessionSearchResult(FOnlineSessionSearchResult InSessionSearchResult)
{
	SessionSearchResult = InSessionSearchResult;

	if (GameNameText)
	{
		GameNameText->SetText(FText::FromString(GetSetting(SETTING_GAMENAME)));
	}

	if (CreatedByText)
	{
		CreatedByText->SetText(FText::FromString(GetSetting(SETTING_CREATEDBY)));
	}

	if (MapNameText)
	{
		MapNameText->SetText(FText::FromString(GetSetting(SETTING_MAPNAME)));
	}

	if (PlayersText)
	{
		PlayersText->SetText(FText::FromString(GetSetting(SETTING_TOTALPLAYERS)));
	}
}

void UArenaChooseGameItem::NativeConstruct()
{
	Super::NativeConstruct();

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UArenaChooseGameItem::OnJoinButtonClicked);
	}
}

void UArenaChooseGameItem::NativeDestruct()
{
	Super::NativeDestruct();

	if (JoinButton)
	{
		JoinButton->OnClicked.RemoveDynamic(this, &UArenaChooseGameItem::OnJoinButtonClicked);
	}
}

void UArenaChooseGameItem::OnJoinSessionCompleted(EOnJoinSessionCompleteResult::Type Result)
{
	UArenaSessionSubsystem* SessionSubsystem = UArenaFunctionLibrary::GetSessionSubsystem(GetWorld());
	if (SessionSubsystem)
	{
		SessionSubsystem->JoinSessionCompleteDelegate.RemoveAll(this);

		if (Result == EOnJoinSessionCompleteResult::Success)
		{
			SessionSubsystem->TravelToCurrentSession();
		}
	}
}

FString UArenaChooseGameItem::GetSetting(const FName& Key)
{
	FString Value;
	SessionSearchResult.Session.SessionSettings.Get(Key, Value);

	return Value;
}

void UArenaChooseGameItem::OnJoinButtonClicked()
{
	UArenaSessionSubsystem* SessionSubsystem = UArenaFunctionLibrary::GetSessionSubsystem(GetWorld());
	if (SessionSubsystem)
	{
		SessionSubsystem->JoinSessionCompleteDelegate.AddUObject(this, &UArenaChooseGameItem::OnJoinSessionCompleted);
		SessionSubsystem->JoinGameSession(SessionSearchResult);
	}
}
