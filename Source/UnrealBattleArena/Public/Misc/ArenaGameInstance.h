// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Online/ArenaOnlineMatch.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ArenaGameInstance.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaGameInstance : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	FORCEINLINE const FOnlinePlayerInfo& GetPlayerInfo() const { return PlayerInfo; }
	FORCEINLINE const FOnlineMatchInfo& GetMatchInfo() const { return MatchInfo; }
	
	virtual void CreateOnlineMatch(const FOnlineSessionSettings& SessionSettings);

protected:

	UPROPERTY(Transient)
	FOnlinePlayerInfo PlayerInfo;

	UPROPERTY(Transient)
	FOnlineMatchInfo MatchInfo;
};
