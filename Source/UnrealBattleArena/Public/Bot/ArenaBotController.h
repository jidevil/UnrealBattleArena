// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Character/ArenaCharacter.h"
#include "ArenaBotController.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaBotController : public AAIController
{
	GENERATED_BODY()

public:

	AArenaBotController();

	AArenaCharacter* GetPerceivedEnemy() const;
	AArenaCharacter* FindClosestEnemy();
	AArenaCharacter* FindClosestEnemyWithLOS();
	AArenaCharacter* FindClosestPerceivedEnemy();
	
	bool HasLOSToEnemy(class AArenaCharacter* Enemy, bool bAnyEnemy) const;
	void ShootEnemy(class AArenaCharacter* Enemy) const;

	bool LowOnAmmo(int32 Threshold) const;
	bool LowOnHealth(float Threshold) const;

protected:

	virtual void OnPossess(APawn* InPawn) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Bot Controller")
	class UBehaviorTree* BehaviorTree{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Bot Controller")
	FName EnemyKeyName;
};
