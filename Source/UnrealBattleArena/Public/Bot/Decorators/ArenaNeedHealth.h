// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ArenaNeedHealth.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaNeedHealth : public UBTDecorator
{
	GENERATED_BODY()

public:

	UArenaNeedHealth();

protected:

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Arena Need Health")
	float Threshold{ 0.0f };
};
