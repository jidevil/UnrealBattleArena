// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArenaPlayerWidget.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetHealth(float Health, float MaxHealth);
	void SetShield(float Shield, float MaxShield);
	void SetStamina(float Stamina);

protected:

	virtual void NativeConstruct() override;

protected:

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HealthProgressBar{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* StaminaProgressBar{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* ShieldProgressBar{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CurrentShieldText{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* MaxShieldText{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CurrentHealthText{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* MaxHealthText{ nullptr };
};
