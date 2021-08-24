// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ArenaPlayerController.h"
#include "Character/ArenaCharacter.h"
#include "Weapons/ArenaWeapon.h"
#include "UI/HUD/ArenaHUD.h"
#include "Blueprint/UserWidget.h"

void AArenaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController())
	{
		PlayerHUD = Cast<UArenaHUD>(CreateWidget(this, PlayerHUDClass));
		if (PlayerHUD)
		{
			PlayerHUD->AddToViewport();
		}
	}
}
