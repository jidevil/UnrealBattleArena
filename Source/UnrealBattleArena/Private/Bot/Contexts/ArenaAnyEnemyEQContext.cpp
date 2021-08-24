// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot/Contexts/ArenaAnyEnemyEQContext.h"
#include "Bot/ArenaBotCharacter.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Kismet/GameplayStatics.h"

void UArenaAnyEnemyEQContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	TArray<AActor*> OutActors;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaCharacter::StaticClass(), OutActors);
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, OutActors);
}
