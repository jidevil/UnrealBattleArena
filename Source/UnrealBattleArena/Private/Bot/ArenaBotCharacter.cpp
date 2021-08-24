// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot/ArenaBotCharacter.h"
#include "Bot/ArenaBotController.h"
#include "Perception/AIPerceptionComponent.h"

AArenaBotCharacter::AArenaBotCharacter()
{
	AIControllerClass = AArenaBotController::StaticClass();
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
}

bool AArenaBotCharacter::IsFirstPersonView() const
{
	return false;
}

void AArenaBotCharacter::FaceRotation(FRotator NewControlRotation, float DeltaTime)
{
	const FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewControlRotation,
		DeltaTime, 8.0f);
	Super::FaceRotation(CurrentRotation, DeltaTime);
}

void AArenaBotCharacter::BeginPlay()
{
	Super::BeginPlay();
}