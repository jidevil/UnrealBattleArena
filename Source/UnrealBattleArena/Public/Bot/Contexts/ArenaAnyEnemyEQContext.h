// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "ArenaAnyEnemyEQContext.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaAnyEnemyEQContext : public UEnvQueryContext
{
	GENERATED_BODY()

protected:

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
