// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ArenaFindPickup.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaFindPickup : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UArenaFindPickup();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arena Find Pickup")
	FBlackboardKeySelector DestinationKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arena Find Pickup")
	TSubclassOf<class AArenaPickup> PickupClass;
};
