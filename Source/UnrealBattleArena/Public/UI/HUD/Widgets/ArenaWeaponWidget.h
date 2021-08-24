// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArenaWeaponWidget.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetAmmo(int32 CurrentAmmo, int32 RemainingAmmo);

protected:

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CurrentAmmoText{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* RemainingAmmoText{ nullptr };
};
