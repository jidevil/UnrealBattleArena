// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/ArenaOnlineMatch.h"
#include "Kismet/GameplayStatics.h"

void UArenaOnlineMatch::NewMatch(const FString& MatchName, const FString& MapName,
	int32 TotalPlayers, EGameType GameType)
{
	MatchInfo.MatchName = MatchName;
	MatchInfo.MapName = MapName;
	MatchInfo.TotalPlayers = TotalPlayers;
	MatchInfo.GameType = GameType;
	MatchInfo.Players.Empty();
}

void UArenaOnlineMatch::JoinPlayer(const FOnlinePlayerInfo& Player)
{
	MatchInfo.Players.Add(Player);

	if (CanStartMatch())
	{
		StartMatch();
	}
}

bool UArenaOnlineMatch::CanStartMatch()
{
	return MatchInfo.TotalPlayers == MatchInfo.Players.Num();
}

void UArenaOnlineMatch::StartMatch()
{
	const FString TravelURL = FString::Printf(TEXT("/Game/UnrealBattleArena/Maps/%s?listen"),
		*MatchInfo.MapName);

	GetWorld()->ServerTravel(TravelURL);
}
