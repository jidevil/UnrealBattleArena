// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/ArenaAmmoAbility.h"
#include "Character/ArenaCharacter.h"
#include "Character/ArenaInventory.h"
#include "Weapons/ArenaWeapon.h"

bool UArenaAmmoAbility::CanStart() const
{
	UArenaInventory* Inventory = ArenaCharacter->GetInventory();
	if (Inventory && Inventory->HasWeapon(WeaponClass))
	{
		AArenaWeapon* Weapon = Inventory->GetWeapon(WeaponClass);
		return Weapon->CanAddClip();
	}

	return false;
}

void UArenaAmmoAbility::OnStarted()
{
	Super::OnStarted();
	
	if (GetOwnerRole() == ROLE_Authority)
	{
		UArenaInventory* Inventory = ArenaCharacter->GetInventory();
		if (Inventory)
		{
			AArenaWeapon* Weapon = Inventory->GetWeapon(WeaponClass);
			if (Weapon)
			{
				Weapon->AddClips(ClipCount);
			}
		}
	}
}
