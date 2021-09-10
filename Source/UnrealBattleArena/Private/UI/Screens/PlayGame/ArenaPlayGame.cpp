// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Screens/PlayGame/ArenaPlayGame.h"
#include "UI/ArenaUIManager.h"
#include "GameModes/ArenaGameModeBase.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/ArenaFunctionLibrary.h"

void UArenaPlayGame::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UArenaPlayGame::OnBackButtonClicked);
	}
}

void UArenaPlayGame::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (BackButton)
	{
		BackButton->OnClicked.RemoveDynamic(this, &UArenaPlayGame::OnBackButtonClicked);
	}	
}

void UArenaPlayGame::OnBackButtonClicked()
{
	UArenaUIManager* UIManager = UArenaFunctionLibrary::GetUIManager(GetWorld());
	if (UIManager)
	{
		UIManager->PopScreen();
	}
}
