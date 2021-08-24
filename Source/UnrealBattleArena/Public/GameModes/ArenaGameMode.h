// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Player/ArenaPlayerState.h"
#include "ArenaGameMode.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	AArenaGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void InitGameState() override;

	virtual bool CanDealDamage(AArenaPlayerState* DamageInstigator, AArenaPlayerState* DamagedPlayer) const;
	virtual int32 ChooseTeam(AArenaPlayerState* PlayerState);

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:

	virtual bool IsSpawnPointAllowed(class APlayerStart* PlayerStart, class AController* Controller) const;
	virtual bool IsSpawnPointPreferred(class APlayerStart* PlayerStart, class AController* Controller) const;
	
protected:

	int32 NumberOfTeams{ 0 };
};
