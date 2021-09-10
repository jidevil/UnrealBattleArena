// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Screens/PlayGame/Widgets/ArenaPlayGameItem.h"
#include "UI/Screens/ChooseGame/ArenaChooseGame.h"
#include "UI/ArenaUIManager.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameModes/ArenaGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/ArenaFunctionLibrary.h"

void UArenaPlayGameItem::NativeConstruct()
{
	Super::NativeConstruct();

	if (GameTypeText)
	{
		GameTypeText->SetText(GameType);
	}

	if (PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &UArenaPlayGameItem::OnPlayButtonClicked);
	}
}

void UArenaPlayGameItem::NativeDestruct()
{
	Super::NativeDestruct();

	if (GameTypeText)
	{
		GameTypeText->SetText(GameType);
	}

	if (PlayButton)
	{
		PlayButton->OnClicked.RemoveDynamic(this, &UArenaPlayGameItem::OnPlayButtonClicked);
	}
}

void UArenaPlayGameItem::OnPlayButtonClicked()
{
	UArenaUIManager* UIManager = UArenaFunctionLibrary::GetUIManager(GetWorld());
	if (UIManager)
	{
		UIManager->PushScreen(ChooseGameClass);
	}
}
