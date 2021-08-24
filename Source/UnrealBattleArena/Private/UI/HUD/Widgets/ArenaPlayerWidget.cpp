// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/Widgets/ArenaPlayerWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UArenaPlayerWidget::SetHealth(float Health, float MaxHealth)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Health);
	}

	if (CurrentHealthText)
	{
		const int32 CurrentHealthInt = static_cast<int32>(Health * 100);
		CurrentHealthText->SetText(FText::AsNumber(CurrentHealthInt));
	}

	if (MaxHealthText)
	{
		const int32 MaxHealthInt = static_cast<int32>(MaxHealth * 100);
		MaxHealthText->SetText(FText::AsNumber(MaxHealthInt));
	}
}

void UArenaPlayerWidget::SetShield(float Shield, float MaxShield)
{
	if (CurrentShieldText)
	{
		CurrentShieldText->SetText(FText::AsNumber(Shield));
	}

	if (MaxShieldText)
	{
		MaxShieldText->SetText(FText::AsNumber(MaxShield));
	}
}

void UArenaPlayerWidget::SetStamina(float Stamina)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Stamina);
	}
}

void UArenaPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
