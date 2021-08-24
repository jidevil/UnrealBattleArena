// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ArenaSearchEnemy.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaSearchEnemy : public UBTService
{
	GENERATED_BODY()

public:

	UArenaSearchEnemy();

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arena Search Enemy")
	FBlackboardKeySelector EnemyKey;
};
