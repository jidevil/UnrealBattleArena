// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot/Services/ArenaSearchEnemy.h"
#include "Bot/ArenaBotController.h"
#include "BehaviorTree/BlackboardComponent.h"

UArenaSearchEnemy::UArenaSearchEnemy()
{
	NodeName = "Search Enemy";
	EnemyKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(
		UArenaSearchEnemy, EnemyKey), AArenaCharacter::StaticClass());
}

void UArenaSearchEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AArenaBotController* BotController = Cast<AArenaBotController>(OwnerComp.GetAIOwner());
	if (BotController)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(EnemyKey.SelectedKeyName,
			BotController->FindClosestPerceivedEnemy());
	}
}
