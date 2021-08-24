// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot/Services/ArenaShootEnemy.h"
#include "Bot/ArenaBotController.h"
#include "Character/ArenaCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UArenaShootEnemy::UArenaShootEnemy()
{
	NodeName = "Shoot Enemy";
	EnemyKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(
		UArenaShootEnemy, EnemyKey), AArenaCharacter::StaticClass());
}

void UArenaShootEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AArenaBotController* BotController = Cast<AArenaBotController>(OwnerComp.GetAIOwner());
	if (!BotController)
	{
		return;
	}

	AArenaCharacter* Enemy = Cast<AArenaCharacter>(OwnerComp.GetBlackboardComponent()->
		GetValueAsObject(EnemyKey.SelectedKeyName));

	BotController->ShootEnemy(Enemy);
}
