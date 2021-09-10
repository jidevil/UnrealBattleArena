// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/ArenaFunctionLibrary.h"
#include "GameModes/ArenaGameModeBase.h"
#include "Online/ArenaSessionSubsystem.h"
#include "Player/ArenaPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Online/ArenaGameState.h"

AArenaGameModeBase* UArenaFunctionLibrary::GetGameMode(const UObject* WorldContextObject)
{
	return Cast<AArenaGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
}

AArenaGameState* UArenaFunctionLibrary::GetGameState(const UObject* WorldContextObject)
{
	return Cast<AArenaGameState>(UGameplayStatics::GetGameState(WorldContextObject));
}

AArenaPlayerController* UArenaFunctionLibrary::GetPlayerController(const UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
		if (World)
		{
			return Cast<AArenaPlayerController>(World->GetFirstPlayerController());
		}
	}

	return nullptr;
}

UArenaSessionSubsystem* UArenaFunctionLibrary::GetSessionSubsystem(const UObject* WorldContextObject)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	if (GameInstance)
	{
		return GameInstance->GetSubsystem<UArenaSessionSubsystem>();
	}

	return nullptr;
}

UArenaUIManager* UArenaFunctionLibrary::GetUIManager(const UObject* WorldContextObject)
{
	AArenaPlayerController* PlayerController = GetPlayerController(WorldContextObject);
	if (PlayerController)
	{
		return PlayerController->GetUIManager();
	}

	return nullptr;
}
