// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/ArenaGameInstance.h"
#include "Online/ArenaSessionSubsystem.h"

void UArenaGameInstance::CreateOnlineMatch(const FOnlineSessionSettings& SessionSettings)
{
	SessionSettings.Get(SETTING_GAMENAME, MatchInfo.MatchName);
	SessionSettings.Get(SETTING_MAPNAME, MatchInfo.MapName);
}
