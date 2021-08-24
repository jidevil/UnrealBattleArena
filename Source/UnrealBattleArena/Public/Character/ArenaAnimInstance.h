// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ArenaAnimInstance.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	void HandleWeaponLeftHandIK();
	void TurnInPlace(float DeltaSeconds);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category="Arena AnimInstance")
	float SpeedForward{ 0.0f };

	UPROPERTY(Transient, BlueprintReadOnly, Category="Arena AnimInstance")
	float SpeedRight{ 0.0f };

	UPROPERTY(Transient, BlueprintReadOnly, Category="Arena AnimInstance")
	float ControllerPitch{ 0.0f };

	UPROPERTY(Transient, BlueprintReadOnly, Category="Arena AnimInstance")
	float ControllerYaw{ 0.0f };
	
	UPROPERTY(Transient, BlueprintReadOnly, Category="Arena AnimInstance")
	bool bIsInAir{ false };

	UPROPERTY(Transient, BlueprintReadOnly, Category="Arena AnimInstance")
	bool bIsSprinting{ false };

	UPROPERTY(Transient, BlueprintReadOnly, Category="Arena AnimInstance")
	bool bIronsights{ false };

	UPROPERTY(Transient, BlueprintReadOnly, Category="Arena AnimInstance")
	bool bTurnInPlace{ false };

	UPROPERTY(Transient, BlueprintReadOnly, Category="Arena AnimInstance")
	float TargetYaw{ false };
	
	UPROPERTY(Transient, BlueprintReadOnly, Category="Arena AnimInstance")
	float LeftHandIKAlpha{ 0.0f };

	UPROPERTY(Transient, BlueprintReadOnly, Category="Arena AnimInstance")
	FTransform LeftHandIKTransform;

	UPROPERTY(Transient, BlueprintReadOnly, Category="Arena AnimInstance")
	bool bIsFullBody{ false };

	UPROPERTY()
	class AArenaCharacter* ArenaCharacter{ nullptr };
};
