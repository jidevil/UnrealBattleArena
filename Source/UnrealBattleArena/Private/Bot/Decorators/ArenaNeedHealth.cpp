// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot/Decorators/ArenaNeedHealth.h"
#include "Bot/ArenaBotController.h"

UArenaNeedHealth::UArenaNeedHealth()
{
	NodeName = "Need Health";
}

bool UArenaNeedHealth::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AArenaBotController* BotController = Cast<AArenaBotController>(OwnerComp.GetAIOwner());
	if (BotController)
	{
		return BotController->LowOnHealth(Threshold);
	}

	return false;
}
