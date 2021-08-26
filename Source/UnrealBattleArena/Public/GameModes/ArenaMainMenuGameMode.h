// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/ArenaGameModeBase.h"
#include "ArenaMainMenuGameMode.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaMainMenuGameMode : public AArenaGameModeBase
{
	GENERATED_BODY()

public:

	AArenaMainMenuGameMode();

protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Main Menu GameMode")
	TSubclassOf<class UArenaMainMenu> MainMenuClass;
};
