// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ArenaWeaponProjectile.h"
#include "Weapons/ArenaWeapon.h"
#include "Character/ArenaCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

AArenaWeaponProjectile::AArenaWeaponProjectile()
{
	SphereBody = CreateDefaultSubobject<USphereComponent>("SphereBody");
	SphereBody->InitSphereRadius(1.0f);
	RootComponent = SphereBody;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 10000.0f;
	ProjectileMovement->MaxSpeed = 12000.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>("TrailEffect");
	TrailEffect->SetupAttachment(SphereBody);
	TrailEffect->bAutoActivate = true;
}

void AArenaWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();	

	Weapon = Cast<AArenaWeapon>(GetOwner());
	if (Weapon)
	{
		const FWeaponAttributes& Attributes = Weapon->Attributes;
		SetLifeSpan(Attributes.BulletRange / ProjectileMovement->InitialSpeed);
		
		SphereBody->OnComponentHit.AddDynamic(this, &AArenaWeaponProjectile::OnProjectileHit);
	}
}

void AArenaWeaponProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AArenaWeaponProjectile, Weapon);
}

void AArenaWeaponProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{	
	if (Weapon && OtherActor)
	{
		const FWeaponAttributes& Attributes = Weapon->Attributes;
		if (Attributes.bProjectileDamage)
		{
			Weapon->SpawnImpactEffects(Hit);

			if (GetLocalRole() == ROLE_Authority)
			{
				const FWeaponDamageAttributes& DamageAttributes = Weapon->DamageAttributes;
				if (DamageAttributes.bRadialDamage)
				{
					TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
					TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
					TArray<AActor*> IgnoreActors;
					IgnoreActors.Add(Weapon->GetCharacter());

					TArray<AActor*> OtherActors;		
					UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Hit.ImpactPoint, DamageAttributes.DamageRadius,
						TraceObjectTypes, AArenaCharacter::StaticClass(), IgnoreActors, OtherActors);

					for (const AActor* Actor : OtherActors)
					{
						const FHitResult HitResult = Weapon->WeaponTrace(Hit.ImpactPoint,
							Actor->GetActorLocation());
						Weapon->ApplyDamage(HitResult);
					}
				}
			}
		}
	}

	Destroy();
}
