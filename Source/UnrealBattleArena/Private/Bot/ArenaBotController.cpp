// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot/ArenaBotController.h"
#include "Bot/ArenaBotCharacter.h"
#include "Character/ArenaCharacter.h"
#include "EngineUtils.h"
#include "UnrealBattleArena.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Weapons/ArenaWeapon.h"

AArenaBotController::AArenaBotController()
{
	bWantsPlayerState = true;
}

AArenaCharacter* AArenaBotController::GetPerceivedEnemy() const
{
	if (Blackboard)
	{
		return Cast<AArenaCharacter>(Blackboard->GetValueAsObject(EnemyKeyName));
	}

	return nullptr;
}

AArenaCharacter* AArenaBotController::FindClosestEnemy()
{
	AArenaBotCharacter* BotCharacter = Cast<AArenaBotCharacter>(GetPawn());
	if (BotCharacter)
	{
		const FVector Location = BotCharacter->GetActorLocation();
		float BestDistSq = MAX_FLT;
		AArenaCharacter* BestEnemy{ nullptr };

		for (AArenaCharacter* Enemy : TActorRange<AArenaCharacter>(GetWorld()))
		{
			if (Enemy->IsAlive() && Enemy->IsEnemyFor(this))
			{
				const float DistSq = (Enemy->GetActorLocation() - Location).SizeSquared();
				if (DistSq < BestDistSq)
				{
					BestDistSq = DistSq;
					BestEnemy = Enemy;
				}
			}
		}

		return BestEnemy;
	}

	return nullptr;
}

AArenaCharacter* AArenaBotController::FindClosestEnemyWithLOS()
{
	AArenaBotCharacter* BotCharacter = Cast<AArenaBotCharacter>(GetPawn());
	if (BotCharacter)
	{
		const FVector Location = BotCharacter->GetActorLocation();
		float BestDistSq = MAX_FLT;
		AArenaCharacter* BestEnemy{ nullptr };

		for (AArenaCharacter* Enemy : TActorRange<AArenaCharacter>(GetWorld()))
		{
			if (Enemy->IsAlive() && Enemy->IsEnemyFor(this) && HasLOSToEnemy(Enemy, true))
			{
				const float DistSq = (Enemy->GetActorLocation() - Location).SizeSquared();
				if (DistSq < BestDistSq)
				{
					BestDistSq = DistSq;
					BestEnemy = Enemy;
				}
			}
		}

		return BestEnemy;
	}
	
	return nullptr;
}

AArenaCharacter* AArenaBotController::FindClosestPerceivedEnemy()
{
	AArenaBotCharacter* BotCharacter = Cast<AArenaBotCharacter>(GetPawn());
	if (BotCharacter)
	{
		TArray<AActor*> PerceivedActors;
		BotCharacter->GetAIPerception()->GetKnownPerceivedActors(nullptr, PerceivedActors);

		const FVector Location = BotCharacter->GetActorLocation();
		float BestDistSq = MAX_FLT;
		AArenaCharacter* BestEnemy{ nullptr };
		
		for (AActor* Actor : PerceivedActors)
		{
			AArenaCharacter* Enemy = Cast<AArenaCharacter>(Actor);
			if (Enemy && Enemy->IsAlive() && Enemy->IsEnemyFor(this))
			{
				const float DistSq = (Enemy->GetActorLocation() - Location).SizeSquared();
				if (DistSq < BestDistSq)
				{
					BestDistSq = DistSq;
					BestEnemy = Enemy;
				}
			}
		}

		return BestEnemy;
	}

	return nullptr;
}

bool AArenaBotController::HasLOSToEnemy(AArenaCharacter* Enemy, bool bAnyEnemy) const
{
	FCollisionQueryParams QueryParams;
	QueryParams.TraceTag = SCENE_QUERY_STAT(BotWeaponLosTrace);
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetPawn());

	FVector StartLocation = GetPawn()->GetActorLocation();
	StartLocation.Z += GetPawn()->BaseEyeHeight;
	const FVector EndLocation = Enemy->GetActorLocation();

	bool bHasLOS = false;
	FHitResult HitResult(ForceInit);
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, COLLISION_WEAPON, QueryParams);

	if (HitResult.bBlockingHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			if (HitActor == Enemy)
			{
				bHasLOS = true;
			}
			else if (bAnyEnemy)
			{
				AArenaCharacter* HitCharacter = Cast<AArenaCharacter>(HitActor);
				if (HitCharacter)
				{
					AArenaCharacter* ThisCharacter = Cast<AArenaCharacter>(GetPawn());
					bHasLOS = ThisCharacter->IsEnemyFor(HitCharacter->GetController());
				}
			}
		}
	}
	
	return bHasLOS;
}

void AArenaBotController::ShootEnemy(AArenaCharacter* Enemy) const
{
	AArenaBotCharacter* BotCharacter = Cast<AArenaBotCharacter>(GetPawn());
	if (BotCharacter)
	{
		AArenaWeapon* CurrentWeapon = BotCharacter->GetCurrentWeapon();
		if (CurrentWeapon)
		{
			if (Enemy && HasLOSToEnemy(Enemy, true))
			{
				CurrentWeapon->BeginFire();	
			}
			else
			{
				CurrentWeapon->EndFire();
			}
		}
	}
}

bool AArenaBotController::LowOnAmmo(int32 Threshold) const
{
	AArenaBotCharacter* BotCharacter = Cast<AArenaBotCharacter>(GetPawn());
	if (BotCharacter)
	{
		AArenaWeapon* CurrentWeapon = BotCharacter->GetCurrentWeapon();
		if (CurrentWeapon)
		{
			return CurrentWeapon->GetRemainingAmmo() <= Threshold;
		}
	}

	return false;
}

bool AArenaBotController::LowOnHealth(float Threshold) const
{
	AArenaBotCharacter* BotCharacter = Cast<AArenaBotCharacter>(GetPawn());
	if (BotCharacter)
	{
		return BotCharacter->GetCurrentHealth() <= Threshold;
	}

	return false;
}

void AArenaBotController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
}
