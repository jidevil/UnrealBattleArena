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
	if (ShieldProgressBar)
	{
		ShieldProgressBar->SetPercent(Shield);	
	}
	
	if (CurrentShieldText)
	{
		const int32 CurrentShieldInt = static_cast<int32>(Shield * 100);
		CurrentShieldText->SetText(FText::AsNumber(CurrentShieldInt));
	}

	if (MaxShieldText)
	{
		const int32 MaxShieldInt = static_cast<int32>(MaxShield * 100);
		MaxShieldText->SetText(FText::AsNumber(MaxShieldInt));
	}	
}

void UArenaPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
