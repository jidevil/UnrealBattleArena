// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "ArenaCameraManager.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:

	AArenaCameraManager();
	virtual void UpdateCamera(float DeltaTime) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Camera Manager")
	float NormalFOV{ 90.0f };
};
