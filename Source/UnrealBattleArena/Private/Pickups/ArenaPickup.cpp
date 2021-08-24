// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/ArenaPickup.h"
#include "Character/ArenaCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

AArenaPickup::AArenaPickup()
{
	bReplicates = true;
	
	SphereBody = CreateDefaultSubobject<USphereComponent>("SphereBody");
	SphereBody->InitSphereRadius(120.0f);
	SphereBody->SetGenerateOverlapEvents(true);
	RootComponent = SphereBody;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(SphereBody);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>("RotatingMovement");
}

void AArenaPickup::BeginPlay()
{
	Super::BeginPlay();
	SphereBody->OnComponentBeginOverlap.AddDynamic(this, &AArenaPickup::OnBeginOverlap);
}

bool AArenaPickup::OnPickedUp(AArenaCharacter* ArenaCharacter)
{
	return false;
}

void AArenaPickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AArenaCharacter* ArenaCharacter = Cast<AArenaCharacter>(OtherActor);
	if (ArenaCharacter)
	{
		if (OnPickedUp(ArenaCharacter))
		{
			Destroy();
		}
	}
}

