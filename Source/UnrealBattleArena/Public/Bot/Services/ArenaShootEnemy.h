// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ArenaShootEnemy.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaShootEnemy : public UBTService
{
	GENERATED_BODY()
	
public:

	UArenaShootEnemy();

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arena Shoot Enemy")
	FBlackboardKeySelector EnemyKey;
};
