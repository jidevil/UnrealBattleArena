// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ArenaNeedAmmo.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaNeedAmmo : public UBTDecorator
{
	GENERATED_BODY()

public:

	UArenaNeedAmmo();

protected:

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arena Need Ammo")
	int32 Threshold{ 0 };
};
