// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ArenaGrenadeLauncher.h"

AArenaGrenadeLauncher::AArenaGrenadeLauncher()
{
	bReplicates = true;
	Attributes.bProjectileDamage = true;
	Attributes.BulletRange = 8000.0f;
	Attributes.BulletSpread = 0.0f;
	Attributes.AmmoPerClip = 4;
	Attributes.MaxAmmoCount = 20;
	Attributes.IronsightsBulletSpread = 0.0f;
	Attributes.IronsightsFOV = 60.0f;
	
	DamageAttributes.bRadialDamage = true;
	DamageAttributes.DamageRadius = 500.0f;
}
