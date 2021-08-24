// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "ArenaPerceivedEnemyEQContext.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaPerceivedEnemyEQContext : public UEnvQueryContext
{
	GENERATED_BODY()
	
protected:

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
