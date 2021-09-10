// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/ArenaMenuItem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UArenaMenuItem::SetText(const FText& Text)
{
	if (MenuText)
	{
		MenuText->SetText(Text);
	}
}

void UArenaMenuItem::NativeConstruct()
{
	Super::NativeConstruct();

	if (MenuText)
	{
		MenuText->SetText(MenuTitle);
	}

	if (MenuButton)
	{
		MenuButton->OnClicked.AddDynamic(this, &UArenaMenuItem::OnMenuItemClicked);
	}
}

void UArenaMenuItem::NativeDestruct()
{
	Super::NativeDestruct();

	if (MenuButton)
	{
		MenuButton->OnClicked.RemoveDynamic(this, &UArenaMenuItem::OnMenuItemClicked);
	}
}

void UArenaMenuItem::OnMenuItemClicked()
{
	if (OnMenuItemClick.IsBound())
	{
		OnMenuItemClick.Broadcast(MenuType);
	}
}
