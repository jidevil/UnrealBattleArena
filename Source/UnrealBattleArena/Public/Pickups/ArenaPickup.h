// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaPickup.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaPickup : public AActor
{
	GENERATED_BODY()
	
public:	

	AArenaPickup();

protected:
	
	virtual void BeginPlay() override;
	virtual bool OnPickedUp(class AArenaCharacter* ArenaCharacter);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Pickup")
	class USphereComponent* SphereBody{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Pickup")
	class UStaticMeshComponent* Mesh{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Pickup")
	class URotatingMovementComponent* RotatingMovement{ nullptr };
};
