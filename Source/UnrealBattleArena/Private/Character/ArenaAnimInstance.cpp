// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ArenaAnimInstance.h"
#include "Character/ArenaCharacter.h"
#include "Weapons/ArenaWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UArenaAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	ArenaCharacter = Cast<AArenaCharacter>(GetOwningActor());	
}

void UArenaAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (ArenaCharacter)
	{
		static constexpr float InterpolationSpeed = 10.0f;
		
		const FVector Velocity = ArenaCharacter->GetVelocity();
		const FVector ForwardVector = ArenaCharacter->GetActorForwardVector();
		const FVector RightVector = ArenaCharacter->GetActorRightVector();
		
		SpeedForward = FVector::DotProduct(Velocity, ForwardVector);
		SpeedRight = FVector::DotProduct(Velocity, RightVector);

		const FRotator LookRotation = ArenaCharacter->IsLocallyControlled() ? ArenaCharacter->GetControlRotation() :
			ArenaCharacter->GetLookRotation();
		const FRotator ActorRotation = ArenaCharacter->GetActorRotation();
		const FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(LookRotation, ActorRotation);

		ControllerPitch = FMath::FInterpTo(ControllerPitch, DeltaRotator.Pitch,	DeltaSeconds, InterpolationSpeed);
		ControllerYaw = FMath::FInterpTo(ControllerYaw, DeltaRotator.Yaw, DeltaSeconds, InterpolationSpeed);
		TargetYaw = DeltaRotator.Yaw;
		
		if (FMath::Abs(DeltaRotator.Yaw) > 89.0f)
		{				
			bTurnInPlace = true;
		}

		if (bTurnInPlace)
		{
			TurnInPlace(DeltaSeconds);
		}

		AArenaWeapon* CurrentWeapon = ArenaCharacter->GetCurrentWeapon();
		if (CurrentWeapon)
		{
			bIronsights = CurrentWeapon->IsInIronsights();
		}
		else
		{
			bIronsights = false;
		}

		bIsFullBody = GetCurveValue("FullBody") > 0.0f;
		bIsInAir = ArenaCharacter->GetCharacterMovement()->MovementMode == MOVE_Falling;
		bIsSprinting = ArenaCharacter->IsSprinting() && Velocity.Size() > 500.0f;
		ArenaCharacter->GetCharacterMovement()->bOrientRotationToMovement = SpeedForward > 0.0f;

		HandleWeaponLeftHandIK();
	}
}

void UArenaAnimInstance::HandleWeaponLeftHandIK()
{
	AArenaWeapon* CurrentWeapon = ArenaCharacter->GetCurrentWeapon();
	if (CurrentWeapon)
	{
		LeftHandIKAlpha = CurrentWeapon->HasLeftHandIK() ? 1.0f : 0.0f;

		USkeletalMeshComponent* PlayerMesh = ArenaCharacter->GetCharacterMesh();
		USkeletalMeshComponent* WeaponMesh = CurrentWeapon->GetMesh();

		if (PlayerMesh && WeaponMesh)
		{
			const FName IKSocketName = bIronsights ?
				(ArenaCharacter->IsFirstPersonView() ? "IK_LeftHand_Ironsights_1P" : "IK_LeftHand_Ironsights_3P") :
				(ArenaCharacter->IsFirstPersonView() ? "IK_LeftHand_1P" : "IK_LeftHand_3P");
						
			const FTransform SocketTransform = WeaponMesh->GetSocketTransform(IKSocketName,
				RTS_World);

			FVector OutPosition;
			FRotator OutRotation;

			PlayerMesh->TransformToBoneSpace("hand_r", SocketTransform.GetLocation(),
				FRotator(), OutPosition, OutRotation);

			LeftHandIKTransform.SetLocation(OutPosition);
		}
	}
	else
	{
		LeftHandIKAlpha = 0.0f;
	}
}

void UArenaAnimInstance::TurnInPlace(float DeltaSeconds)
{
	const float DeltaYaw = FMath::FInterpTo(0.0f, TargetYaw, DeltaSeconds, 5.0f);
	FRotator DeltaRotator(0.0f, DeltaYaw, 0.0f);
	
	if (FMath::IsNearlyEqual(TargetYaw, 0.0f, 2.0f))
	{
		DeltaRotator.Yaw = TargetYaw;
		bTurnInPlace = false;
	}
	
	ArenaCharacter->AddActorWorldRotation(DeltaRotator);
}
