// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/ArenaGameState.h"
#include "Online/ArenaOnlineMatch.h"

void AArenaGameState::CreateMatch(const FString& GameName, const FString& MapName, int32 TotalPlayers)
{
	OnlineMatch = NewObject<UArenaOnlineMatch>(this);
	OnlineMatch->NewMatch(GameName, MapName, TotalPlayers, EGameType::Coop);
}

void AArenaGameState::JoinMatch(const FString& PlayerName, bool bMatchOwner)
{
	if (OnlineMatch)
	{
		OnlineMatch->JoinPlayer({ PlayerName, bMatchOwner });
	}
}

void AArenaGameState::BeginPlay()
{
	Super::BeginPlay();
}
