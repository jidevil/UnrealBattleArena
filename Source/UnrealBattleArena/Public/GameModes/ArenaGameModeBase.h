// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArenaGameModeBase.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AArenaGameModeBase();

protected:

	virtual void BeginPlay() override;
};
