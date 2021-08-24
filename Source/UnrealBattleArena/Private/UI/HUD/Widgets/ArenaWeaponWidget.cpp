// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/Widgets/ArenaWeaponWidget.h"
#include "Components/TextBlock.h"

void UArenaWeaponWidget::SetAmmo(int32 CurrentAmmo, int32 RemainingAmmo)
{
	if (CurrentAmmoText)
	{
		CurrentAmmoText->SetText(FText::AsNumber(CurrentAmmo));
	}

	if (RemainingAmmoText)
	{
		RemainingAmmoText->SetText(FText::AsNumber(RemainingAmmo));
	}
}
