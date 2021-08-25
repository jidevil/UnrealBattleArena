// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArenaFloatingHealth.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaFloatingHealth : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetHealth(float Health);
	void SetShield(float Shield);

protected:

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HealthProgressBar{ nullptr };
	
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* ShieldProgressBar{ nullptr };
};
