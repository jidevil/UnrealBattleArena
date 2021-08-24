// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/ArenaHealthAbility.h"
#include "Character/ArenaCharacter.h"

bool UArenaHealthAbility::CanStart() const
{
	return ArenaCharacter->GetCurrentHealth() < ArenaCharacter->GetAttributes().MaxHealth;
}

void UArenaHealthAbility::OnStarted()
{
	Super::OnStarted();

	if (GetOwnerRole() == ROLE_Authority)
	{
		ArenaCharacter->AddHealth(Health);
	}
}
