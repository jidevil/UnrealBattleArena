// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Screens/ArenaMainMenu.h"
#include "UI/Widgets/ArenaMenuItem.h"
#include "Components/VerticalBox.h"

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
}

void UArenaMainMenu::OnOptions()
{
}

void UArenaMainMenu::OnExit()
{
}
