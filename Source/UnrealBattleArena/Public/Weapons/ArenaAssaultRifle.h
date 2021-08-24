// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/ArenaWeapon.h"
#include "ArenaAssaultRifle.generated.h"

USTRUCT(BlueprintType)
struct FAssaultRifleAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FireRate{ 0.1f };
};

UCLASS()
class UNREALBATTLEARENA_API AArenaAssaultRifle : public AArenaWeapon
{
	GENERATED_BODY()

public:

	AArenaAssaultRifle();

	virtual void BeginFire() override;
	virtual void EndFire() override;

protected:

	virtual void StopFire() override;
	virtual void OnReloadFinished() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Assault Rifle")
	FAssaultRifleAttributes AssaultRifleAttributes;

	FTimerHandle AutomaticTimer;
	bool bStillFiring{ false };
};
