// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/ArenaGameMode.h"
#include "ArenaCoopGameMode.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaCoopGameMode : public AArenaGameMode
{
	GENERATED_BODY()

public:

	virtual int32 ChooseTeam(AArenaPlayerState* PlayerState) override;

protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY()
	class AArenaWaveSystem* WaveSystem{ nullptr };
};
