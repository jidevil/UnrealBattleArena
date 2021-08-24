// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ArenaPlayerState.h"
#include "Net/UnrealNetwork.h"

AArenaPlayerState::AArenaPlayerState()
{
	bReplicates = true;
}

void AArenaPlayerState::SetTeamNumber(int32 InTeamNumber)
{
	TeamNumber = InTeamNumber;
}

void AArenaPlayerState::SetIsBot(bool bInIsBot)
{
	bIsBot = bInIsBot;
}

void AArenaPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AArenaPlayerState, TeamNumber);
	DOREPLIFETIME(AArenaPlayerState, bIsBot);
}
