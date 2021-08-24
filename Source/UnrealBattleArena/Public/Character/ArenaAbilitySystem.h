// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArenaAbilitySystem.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNREALBATTLEARENA_API UArenaAbilitySystem : public UActorComponent
{
	GENERATED_BODY()

public:	

	UArenaAbilitySystem();

	bool IsAbilityRunning(TSubclassOf<class UArenaAbility> AbilityClass);
	bool AddAbility(TSubclassOf<class UArenaAbility> AbilityClass, bool bStart = true);

protected:

	UFUNCTION()
	void OnAbilityStarted(class UArenaAbility* Ability);

	UFUNCTION()
	void OnAbilityEnded(class UArenaAbility* Ability);
	
protected:

	UPROPERTY(Replicated)
	TArray<class UArenaAbility*> RunningAbilities;
};
