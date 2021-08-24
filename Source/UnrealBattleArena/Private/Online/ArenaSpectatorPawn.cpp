// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/ArenaSpectatorPawn.h"

AArenaSpectatorPawn::AArenaSpectatorPawn()
{
	bReplicates = false;
}

void AArenaSpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADefaultPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADefaultPawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ADefaultPawn::MoveUp_World);
	PlayerInputComponent->BindAxis("Turn", this, &ADefaultPawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ADefaultPawn::AddControllerPitchInput);
}
