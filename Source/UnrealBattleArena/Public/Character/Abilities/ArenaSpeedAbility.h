// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArenaAbility.h"
#include "ArenaSpeedAbility.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaSpeedAbility : public UArenaAbility
{
	GENERATED_BODY()

protected:

	virtual void OnStarted() override;
	virtual void OnStopped() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Speed Ability")
	float SpeedMultiplier{ 1.0f };
};
