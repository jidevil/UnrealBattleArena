// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "ArenaSpectatorPawn.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()
	
public:

	AArenaSpectatorPawn();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
