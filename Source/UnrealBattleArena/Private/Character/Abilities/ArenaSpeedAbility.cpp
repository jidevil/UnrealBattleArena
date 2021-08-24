// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/ArenaSpeedAbility.h"
#include "Character/ArenaCharacter.h"

void UArenaSpeedAbility::OnStarted()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		ArenaCharacter->AddSpeedMultiplier(SpeedMultiplier);
	}
}

void UArenaSpeedAbility::OnStopped()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		ArenaCharacter->AddSpeedMultiplier(1.0f / SpeedMultiplier);
	}
}
