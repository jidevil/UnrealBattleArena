// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArenaCharacter.h"
#include "ArenaPlayerCharacter.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaPlayerCharacter : public AArenaCharacter
{
	GENERATED_BODY()

public:

	virtual void GetAimLocationAndRotaion(FVector& AimLocation, FVector& AimRotation) const override;
};
