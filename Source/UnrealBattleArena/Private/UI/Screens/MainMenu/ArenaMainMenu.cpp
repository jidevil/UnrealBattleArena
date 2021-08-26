// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Screens/MainMenu/ArenaMainMenu.h"
#include "UI/Screens/CreateGame/ArenaCreateGame.h"
#include "UI/Widgets/ArenaMenuItem.h"
#include "UI/ArenaUIManager.h"
#include "Components/VerticalBox.h"
#include "GameModes/ArenaGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UArenaMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (MenuVerticalBox)
	{
		const int32 MenuItemCount = MenuVerticalBox->GetChildrenCount();
		for (int32 Index = 0; Index < MenuItemCount; Index++)
		{
			UArenaMenuItem* MenuItem = Cast<UArenaMenuItem>(MenuVerticalBox->GetChildAt(Index));
			if (MenuItem)
			{
				MenuItem->OnMenuItemClick.AddDynamic(this, &UArenaMainMenu::OnMenuItemClicked);
			}
		}
	}
}

void UArenaMainMenu::OnMenuItemClicked(const FName& MenuType)
{
	if (MenuType == "PlayGame")
	{
		OnPlayGame();
	}
	else if (MenuType == "Options")
	{
		OnOptions();
	}
	else if (MenuType == "Exit")
	{
		OnExit();
	}
}

void UArenaMainMenu::OnPlayGame()
{
	AArenaGameModeBase* GameMode = Cast<AArenaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode && GameMode->GetUIManager())
	{
		GameMode->GetUIManager()->PushScreen(CreateGameClass);
	}
}

void UArenaMainMenu::OnOptions()
{
}

void UArenaMainMenu::OnExit()
{
}
