// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArenaAbility.h"
#include "ArenaShieldAbility.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaShieldAbility : public UArenaAbility
{
	GENERATED_BODY()
	
public:
	
	virtual bool CanStart() const override;

protected:

	virtual void OnStarted() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Shield Ability")
	float Shield{ 0.0f };	
};
