// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/ArenaShieldAbility.h"
#include "Character/ArenaCharacter.h"

bool UArenaShieldAbility::CanStart() const
{
	return ArenaCharacter->GetCurrentShield() < ArenaCharacter->GetAttributes().MaxShield;
}

void UArenaShieldAbility::OnStarted()
{
	Super::OnStarted();

	if (GetOwnerRole() == ROLE_Authority)
	{
		ArenaCharacter->AddShield(Shield);
	}
}
