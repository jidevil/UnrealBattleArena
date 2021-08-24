// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArenaAbility.generated.h"

class UArenaAbility;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FAbilityStartedSignature, UArenaAbility, OnAbilityStarted,
	UArenaAbility*, Ability);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FAbilityEndedSignature, UArenaAbility, OnAbilityEnded,
	UArenaAbility*, Ability);

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	Instant UMETA(DisplayName="Instant"),
	Duration UMETA(DisplayName="Duration")
};

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNREALBATTLEARENA_API UArenaAbility : public UActorComponent
{
	GENERATED_BODY()

public:	

	UArenaAbility();

	virtual bool CanStart() const;
	virtual bool Start();

protected:
	
	virtual void BeginPlay() override;
	virtual void RunAbility();
	virtual void OnStarted();
	virtual void OnStopped();

	virtual void SetupInput();
	virtual void StartEffects();
	virtual void StopEffects();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStart();

	UFUNCTION()
	void OnRep_Started();

public:

	UPROPERTY(Transient, BlueprintReadOnly, Category="Arena Ability")
	FAbilityStartedSignature OnAbilityStarted;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category="Arena Ability")
	FAbilityEndedSignature OnAbilityEnded;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Ability")
	EAbilityType AbilityType{ EAbilityType::Instant };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Ability")
	FName InputAction{ NAME_None };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Ability")
	float Duration{ 0.0f };

	UPROPERTY(ReplicatedUsing=OnRep_Started)
	bool bStarted{ false };
	
	UPROPERTY(Replicated)
	class AArenaCharacter* ArenaCharacter{ nullptr };
};
