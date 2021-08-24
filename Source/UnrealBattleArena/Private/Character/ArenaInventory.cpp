// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ArenaInventory.h"
#include "Character/ArenaCharacter.h"
#include "Weapons/ArenaWeapon.h"
#include "Net/UnrealNetwork.h"

UArenaInventory::UArenaInventory()
{
	SetIsReplicatedByDefault(true);
}

void UArenaInventory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UArenaInventory, Weapons);
	DOREPLIFETIME(UArenaInventory, Character);
}

bool UArenaInventory::HasWeapon(TSubclassOf<AArenaWeapon> WeaponClass) const
{
	for (const AArenaWeapon* Weapon : Weapons)
	{
		if (Weapon->GetClass() == WeaponClass)
		{
			return true;
		}
	}

	return false;
}

int32 UArenaInventory::GetNumWeapons() const
{
	return Weapons.Num();
}

AArenaWeapon* UArenaInventory::GetWeapon(int32 WeaponIndex) const
{
	if (WeaponIndex >= 0 && WeaponIndex < Weapons.Num())
	{
		return Weapons[WeaponIndex];
	}

	return nullptr;
}

AArenaWeapon* UArenaInventory::GetWeapon(TSubclassOf<AArenaWeapon> WeaponClass) const
{
	for (AArenaWeapon* Weapon : Weapons)
	{
		if (Weapon->GetClass() == WeaponClass)
		{
			return Weapon;
		}
	}

	return nullptr;
}

void UArenaInventory::AddWeapon(AArenaWeapon* Weapon)
{
	if (!HasWeapon(Weapon->GetClass()))
	{
		Weapons.Add(Weapon);
	}
}

void UArenaInventory::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		Character = Cast<AArenaCharacter>(GetOwner());
	}
}