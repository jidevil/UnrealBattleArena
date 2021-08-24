// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ArenaPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Player/ArenaPlayerController.h"
#include "UI/HUD/ArenaHUD.h"
#include "UI/HUD/Widgets/ArenaPlayerWidget.h"

void AArenaPlayerCharacter::GetAimLocationAndRotation(FVector& AimLocation, FVector& AimRotation) const
{
	AimLocation = GetCamera()->GetComponentLocation();
	AimRotation = GetCamera()->GetForwardVector();
}
