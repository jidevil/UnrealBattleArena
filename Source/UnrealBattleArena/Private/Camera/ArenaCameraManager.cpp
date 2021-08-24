// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/ArenaCameraManager.h"

#include "Character/ArenaCharacter.h"
#include "Weapons/ArenaWeapon.h"

AArenaCameraManager::AArenaCameraManager()
{
	ViewPitchMin = -87.0f;
	ViewPitchMax = 87.0f;
	bAlwaysApplyModifiers = true;
}

void AArenaCameraManager::UpdateCamera(float DeltaTime)
{
	AArenaCharacter* ArenaCharacter = PCOwner ? Cast<AArenaCharacter>(PCOwner) : nullptr;
	if (ArenaCharacter && ArenaCharacter->IsFirstPersonView())
	{
		AArenaWeapon* CurrentWeapon = ArenaCharacter->GetCurrentWeapon();
		const float TargetFOV = (CurrentWeapon && CurrentWeapon->IsInIronsights()) ?
			CurrentWeapon->GetAttributes().IronsightsFOV : NormalFOV;

		DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFOV, DeltaTime, 20.0f);
	}

	Super::UpdateCamera(DeltaTime);
}
