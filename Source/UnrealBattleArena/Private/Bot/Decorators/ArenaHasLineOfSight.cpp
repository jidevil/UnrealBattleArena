// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot/Decorators/ArenaHasLineOfSight.h"
#include "Bot/ArenaBotController.h"
#include "Character/ArenaCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UArenaHasLineOfSight::UArenaHasLineOfSight()
{
	NodeName = "Has Line Of Sight";
	EnemyKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(
		UArenaHasLineOfSight, EnemyKey), AArenaCharacter::StaticClass());
}

bool UArenaHasLineOfSight::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AArenaBotController* BotController = Cast<AArenaBotController>(OwnerComp.GetAIOwner());
	if (!BotController)
	{
		return false;
	}

	AArenaCharacter* Enemy = Cast<AArenaCharacter>(OwnerComp.GetBlackboardComponent()->
		GetValueAsObject(EnemyKey.SelectedKeyName));

	if (!Enemy)
	{
		return false;
	}

	return BotController->HasLOSToEnemy(Enemy, true);
}
