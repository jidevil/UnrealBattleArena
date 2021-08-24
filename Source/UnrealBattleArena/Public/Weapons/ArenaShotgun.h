// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/ArenaWeapon.h"
#include "ArenaShotgun.generated.h"

USTRUCT(BlueprintType)
struct FShotgunAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BurstCount{ 4 };
};

UCLASS()
class UNREALBATTLEARENA_API AArenaShotgun : public AArenaWeapon
{
	GENERATED_BODY()

public:

	AArenaShotgun();

protected:

	virtual void Fire(const FVector& AimLocation, const FVector& AimRotation) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Shotgun")
	FShotgunAttributes ShotgunAttributes;
};
