// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArenaHUD.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	FORCEINLINE class UArenaPlayerWidget* GetPlayerWidget() const { return PlayerWidget; }
	FORCEINLINE class UArenaWeaponWidget* GetWeaponWidget() const { return WeaponWidget; }

protected:

	UPROPERTY(meta=(BindWidget))
	class UArenaPlayerWidget* PlayerWidget{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UArenaWeaponWidget* WeaponWidget{ nullptr };
};
