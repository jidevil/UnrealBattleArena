// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArenaCharacter.h"
#include "ArenaBotCharacter.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaBotCharacter : public AArenaCharacter
{
	GENERATED_BODY()

public:

	AArenaBotCharacter();

	FORCEINLINE class UAIPerceptionComponent* GetAIPerception() const { return AIPerception; }

	virtual bool IsFirstPersonView() const override;
	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	virtual void OnRep_CurrentHealth() override;
	virtual void OnRep_CurrentShield() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Bot Character")
	class UAIPerceptionComponent* AIPerception{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Bot Character")
	class UWidgetComponent* FloatingHealth{ nullptr };

	UPROPERTY(Transient)
	class UArenaFloatingHealth* HealthWidget{ nullptr };
};
