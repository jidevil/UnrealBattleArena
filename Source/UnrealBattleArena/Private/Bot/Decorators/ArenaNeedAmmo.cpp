// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot/Decorators/ArenaNeedAmmo.h"

#include "Bot/ArenaBotController.h"

UArenaNeedAmmo::UArenaNeedAmmo()
{
	NodeName = "Need Ammo";
}

bool UArenaNeedAmmo::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AArenaBotController* BotController = Cast<AArenaBotController>(OwnerComp.GetAIOwner());
	if (BotController)
	{
		return BotController->LowOnAmmo(Threshold);
	}

	return false;
}
