// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/ArenaGameModeBase.h"
#include "ArenaLobbyGameMode.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaLobbyGameMode : public AArenaGameModeBase
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
};
