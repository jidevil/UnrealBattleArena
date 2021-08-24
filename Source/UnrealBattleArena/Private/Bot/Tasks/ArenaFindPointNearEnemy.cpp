// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot/Tasks/ArenaFindPointNearEnemy.h"
#include "Bot/ArenaBotController.h"
#include "Character/ArenaCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UArenaFindPointNearEnemy::UArenaFindPointNearEnemy()
{
	NodeName = "Find Point Near Enemy";
	DestinationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(
		UArenaFindPointNearEnemy, DestinationKey));

	EnemyKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(
		UArenaFindPointNearEnemy, EnemyKey), AArenaCharacter::StaticClass());
}

EBTNodeResult::Type UArenaFindPointNearEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AArenaBotController* BotController = Cast<AArenaBotController>(OwnerComp.GetAIOwner());
	if (BotController)
	{
		APawn* Bot = BotController->GetPawn();
		if (Bot)
		{
			AArenaCharacter* Enemy =  Cast<AArenaCharacter>(OwnerComp.GetBlackboardComponent()->
				GetValueAsObject(EnemyKey.SelectedKeyName));
			
			if (!Enemy)
			{
				Enemy = BotController->FindClosestEnemyWithLOS();
				if (!Enemy)
				{
					Enemy = BotController->FindClosestEnemy();
				}
			}

			if (Enemy)
			{
				constexpr float SearchRadius = 300.0f;
				const FVector SearchOrigin = Enemy->GetActorLocation() + 600 * (Bot->GetActorLocation() -
					Enemy->GetActorLocation()).GetSafeNormal();

				FVector Location;
				UNavigationSystemV1::K2_GetRandomReachablePointInRadius(BotController, SearchOrigin,
					Location, SearchRadius);

				if (Location != FVector::ZeroVector)
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(DestinationKey.SelectedKeyName, Location);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}
