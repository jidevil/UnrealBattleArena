// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot/ArenaBotCharacter.h"
#include "Bot/ArenaBotController.h"
#include "Components/WidgetComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "UI/Widgets/ArenaFloatingHealth.h"

AArenaBotCharacter::AArenaBotCharacter()
{
	AIControllerClass = AArenaBotController::StaticClass();
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
	
	FloatingHealth = CreateDefaultSubobject<UWidgetComponent>("FloatingHealth");
	FloatingHealth->SetupAttachment(RootComponent);
}

void AArenaBotCharacter::Die()
{
	if (HealthWidget)
	{
		HealthWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
	Super::Die();
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
	HealthWidget = Cast<UArenaFloatingHealth>(FloatingHealth->GetWidget());

	CurrentShield = 0.0f;
	if (HealthWidget)
	{
		HealthWidget->SetShield(CurrentShield);
	}
}

void AArenaBotCharacter::OnRep_CurrentHealth()
{
	if (HealthWidget)
	{
		HealthWidget->SetHealth(CurrentHealth);
	}
}

void AArenaBotCharacter::OnRep_CurrentShield()
{
	if (HealthWidget)
	{
		HealthWidget->SetShield(CurrentShield);
	}
}
