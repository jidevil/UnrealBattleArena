// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot/Contexts/ArenaPerceivedEnemyEQContext.h"
#include "Bot/ArenaBotCharacter.h"
#include "Bot/ArenaBotController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UArenaPerceivedEnemyEQContext::ProvideContext(FEnvQueryInstance& QueryInstance,
	FEnvQueryContextData& ContextData) const
{
	AArenaBotCharacter* BotCharacter = Cast<AArenaBotCharacter>(QueryInstance.Owner.Get());
	if (!BotCharacter)
	{
		return;	
	}

	AArenaBotController* BotController = Cast<AArenaBotController>(BotCharacter->GetController());
	if (BotController)
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, BotController->GetPerceivedEnemy());   
	}
}
