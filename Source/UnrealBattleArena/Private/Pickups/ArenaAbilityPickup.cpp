// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/ArenaAbilityPickup.h"
#include "Character/ArenaAbilitySystem.h"
#include "Character/ArenaCharacter.h"

bool AArenaAbilityPickup::OnPickedUp(AArenaCharacter* ArenaCharacter)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		UArenaAbilitySystem* AbilitySystem = ArenaCharacter->GetAbilitySystem();
		if (AbilitySystem)
		{
			return AbilitySystem->AddAbility(AbilityClass, bAutoStart);
		}
	}

	return false;
}
