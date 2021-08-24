// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot/Tasks/ArenaFindPickup.h"
#include "Bot/ArenaBotCharacter.h"
#include "Bot/ArenaBotController.h"
#include "Pickups/ArenaPickup.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UArenaFindPickup::UArenaFindPickup()
{
	NodeName = "Find Pickup";
	DestinationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UArenaFindPickup, DestinationKey));
}

EBTNodeResult::Type UArenaFindPickup::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AArenaBotController* BotController = Cast<AArenaBotController>(OwnerComp.GetAIOwner());
	if (!BotController)
	{
		return EBTNodeResult::Failed;
	}

	AArenaBotCharacter* BotCharacter = Cast<AArenaBotCharacter>(BotController->GetPawn());
	if (!BotCharacter)
	{
		return EBTNodeResult::Failed;
	}
	
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PickupClass, OutActors);

	AActor* BestPickup{ nullptr };
	float BestDist{ MAX_FLT };

	for (AActor* Pickup : OutActors)
	{
		const float Distance = (BotCharacter->GetActorLocation() - Pickup->GetActorLocation()).SizeSquared();
		if (Distance < BestDist)
		{
			BestDist = Distance;
			BestPickup = Pickup;
		}
	}

	if (BestPickup)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(DestinationKey.SelectedKeyName,
			BestPickup->GetActorLocation());
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
