// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ArenaShotgun.h"

AArenaShotgun::AArenaShotgun()
{
	bReplicates = true;
	Attributes.bProjectileDamage = false;
	Attributes.BulletRange = 6000.0f;
	Attributes.BulletSpread = 4.0f;
	Attributes.AmmoPerClip = 6;
	Attributes.MaxAmmoCount = 30;
	Attributes.IronsightsBulletSpread = 2.0f;
	Attributes.IronsightsFOV = 60.0f;
}

void AArenaShotgun::Fire(const FVector& AimLocation, const FVector& AimRotation)
{
	for (int32 Index = 0; Index < ShotgunAttributes.BurstCount; Index++)
	{
		Super::Fire(AimLocation, AimRotation);
	}
}