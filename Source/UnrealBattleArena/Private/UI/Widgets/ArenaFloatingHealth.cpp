// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/ArenaFloatingHealth.h"
#include "Components/ProgressBar.h"

void UArenaFloatingHealth::SetHealth(float Health)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Health);
	}
}

void UArenaFloatingHealth::SetShield(float Shield)
{
	if (ShieldProgressBar)
	{
		ShieldProgressBar->SetPercent(Shield);
	}
}
