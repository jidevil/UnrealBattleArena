// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/ArenaGameModeBase.h"
#include "Player/ArenaPlayerState.h"
#include "ArenaGameMode.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaGameMode : public AArenaGameModeBase
{
	GENERATED_BODY()

public:

	AArenaGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void InitGameState() override;

	virtual void PlayerDied(AController* Controller);
	virtual void RespawnPlayer(AController* Controller);

	virtual bool CanDealDamage(AArenaPlayerState* DamageInstigator, AArenaPlayerState* DamagedPlayer) const;
	virtual int32 ChooseTeam(AArenaPlayerState* PlayerState);
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:

	virtual bool IsSpawnPointAllowed(class APlayerStart* PlayerStart, class AController* Controller) const;
	virtual bool IsSpawnPointPreferred(class APlayerStart* PlayerStart, class AController* Controller) const;
	virtual float GetRespawnTime(AController* Controller);
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena GameMode")
	float DefaultRespawnTime{ 1.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena GameMode")
	int32 NumberOfTeams{ 0 };
};
