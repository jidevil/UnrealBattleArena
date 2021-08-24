// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ArenaPlayerController.h"
#include "Character/ArenaCharacter.h"
#include "Weapons/ArenaWeapon.h"
#include "UI/HUD/ArenaHUD.h"
#include "Blueprint/UserWidget.h"

void AArenaPlayerController::PawnPendingDestroy(APawn* InPawn)
{
	const FVector CameraLocation = InPawn->GetActorLocation() + FVector(0.0f, 0.0f, 300.0f);
	const FRotator CameraRotation(-90.0f, 0.0f, 0.0f);
	
	Super::PawnPendingDestroy(InPawn);
	ClientSetSpectatorCamera(CameraLocation, CameraRotation);
}

void AArenaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetLocalRole() < ROLE_Authority)
	{
		PlayerHUD = Cast<UArenaHUD>(CreateWidget(this, PlayerHUDClass));
		if (PlayerHUD)
		{
			PlayerHUD->AddToViewport();
		}
	}
}

void AArenaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AArenaPlayerController::UnFreeze()
{
	ServerRestartPlayer();
}

void AArenaPlayerController::ClientSetSpectatorCamera_Implementation(FVector CameraLocation, FRotator CameraRotation)
{
	SetInitialLocationAndRotation(CameraLocation, CameraRotation);
	SetViewTarget(this);
}
