// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArenaAbility.h"
#include "ArenaAmmoAbility.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaAmmoAbility : public UArenaAbility
{
	GENERATED_BODY()

public:

	virtual bool CanStart() const override;

protected:

	virtual void OnStarted() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Ammo Ability")
	TSubclassOf<class AArenaWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Ammo Ability");
	int32 ClipCount{ 1 };
};
