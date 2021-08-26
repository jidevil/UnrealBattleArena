// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Screens/CreateGame/ArenaCreateGame.h"
#include "UI/Widgets/ArenaSelectList.h"
#include "Online/ArenaSessionSubsystem.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UArenaCreateGame::NativeConstruct()
{
	Super::NativeConstruct();

	if (MapNameSelectList)
	{
		MapNameSelectList->AddItem(FText::FromString("Map_Level01"), "Map_Level01");
		MapNameSelectList->SetCurrentItem(0);
	}

	if (PlayersSelectList)
	{
		PlayersSelectList->AddItem(FText::FromString("1"), "1");
		PlayersSelectList->AddItem(FText::FromString("2"), "2");
		PlayersSelectList->AddItem(FText::FromString("3"), "3");
		PlayersSelectList->AddItem(FText::FromString("4"), "4");
		PlayersSelectList->AddItem(FText::FromString("5"), "5");
		PlayersSelectList->SetCurrentItem(0);
	}

	if (CreateButton)
	{
		CreateButton->OnClicked.AddDynamic(this, &UArenaCreateGame::OnCreateButtonClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UArenaCreateGame::OnCancelButtonClicked);
	}
}

void UArenaCreateGame::OnCreateSessionCompleted(bool bSuccessful)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (GameInstance)
	{
		UArenaSessionSubsystem* SessionSubsystem = GameInstance->GetSubsystem<UArenaSessionSubsystem>();
		if (SessionSubsystem)
		{
			SessionSubsystem->CreateSessionCompleteDelegate.RemoveDynamic(this,
				&UArenaCreateGame::OnCreateSessionCompleted);
		}
	}

	if (bSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("New session created successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error creating session"));
	}
}

void UArenaCreateGame::OnCreateButtonClicked()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (GameInstance)
	{
		UArenaSessionSubsystem* SessionSubsystem = GameInstance->GetSubsystem<UArenaSessionSubsystem>();
		if (SessionSubsystem)
		{
			UE_LOG(LogTemp, Warning, TEXT("Creating new game session"));
			SessionSubsystem->CreateSessionCompleteDelegate.AddDynamic(this,
				&UArenaCreateGame::OnCreateSessionCompleted);
			
			const FSelectItem& SelectedMapItem = MapNameSelectList->GetSelectedItem();
			SessionSubsystem->CreateSession(10, false,
				SelectedMapItem.Value);
		}
	}
}

void UArenaCreateGame::OnCancelButtonClicked()
{
}
