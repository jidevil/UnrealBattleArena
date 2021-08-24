// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ArenaFindPointNearEnemy.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaFindPointNearEnemy : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UArenaFindPointNearEnemy();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arena Find Point Near Enemy")
	FBlackboardKeySelector DestinationKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arena Find Point Near Enemy")
	FBlackboardKeySelector EnemyKey;
};
