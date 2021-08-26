// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Screens/ChooseGame//Widgets/ArenaGameItem.h"
#include "Components/TextBlock.h"

void UArenaGameItem::SetGameName(const FText& GameName)
{
	if (GameNameText)
	{
		GameNameText->SetText(GameName);
	}
}

void UArenaGameItem::SetCreatedBy(const FText& CreatedBy)
{
	if (CreatedByText)
	{
		CreatedByText->SetText(CreatedBy);
	}
}

void UArenaGameItem::SetMapName(const FText& MapName)
{
	if (MapNameText)
	{
		MapNameText->SetText(MapName);
	}
}

void UArenaGameItem::SetPlayers(const FText& Players)
{
	if (PlayersText)
	{
		PlayersText->SetText(Players);
	}
}

void UArenaGameItem::SetSessionId(const FString& InSessionId)
{
	SessionId = InSessionId;
}
