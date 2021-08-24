// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ArenaAssaultRifle.h"


AArenaAssaultRifle::AArenaAssaultRifle()
{
	bReplicates = true;
	Attributes.bProjectileDamage = false;
	Attributes.BulletRange = 15000.0f;
	Attributes.BulletSpread = 3.0f;
	Attributes.AmmoPerClip = 40;
	Attributes.MaxAmmoCount = 200;
	Attributes.IronsightsBulletSpread = 1.0f;
	Attributes.IronsightsFOV = 60.0f;
}

void AArenaAssaultRifle::BeginFire()
{
	bStillFiring = true;
	FireCount = 0;
	
	GetWorldTimerManager().SetTimer(AutomaticTimer, this, &AArenaAssaultRifle::OnFire,
		AssaultRifleAttributes.FireRate, true);
}

void AArenaAssaultRifle::EndFire()
{
	bStillFiring = false;
	StopFire();
}

void AArenaAssaultRifle::StopFire()
{
	Super::StopFire();
	GetWorldTimerManager().ClearTimer(AutomaticTimer);
}

void AArenaAssaultRifle::OnReloadFinished()
{
	Super::OnReloadFinished();

	if (bStillFiring)
	{
		BeginFire();
	}
}

