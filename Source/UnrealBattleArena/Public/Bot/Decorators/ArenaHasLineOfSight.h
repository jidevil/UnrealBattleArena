// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ArenaHasLineOfSight.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaHasLineOfSight : public UBTDecorator
{
	GENERATED_BODY()

public:

	UArenaHasLineOfSight();

protected:

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arena Has Line Of Sight")
	FBlackboardKeySelector EnemyKey;
};
