// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/ArenaPickup.h"
#include "ArenaAbilityPickup.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaAbilityPickup : public AArenaPickup
{
	GENERATED_BODY()

protected:

	virtual bool OnPickedUp(AArenaCharacter* ArenaCharacter) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Ability Pickup")
	TSubclassOf<class UArenaAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Ability Pickup")
	bool bAutoStart{ true };
};
