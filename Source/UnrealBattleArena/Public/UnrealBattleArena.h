// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define CHARACTER_DEFAULT_WALK_SPEED 400.0f
#define CHARACTER_DEFAULT_CROUCH_SPEED 200.0f
#define CHARACTER_DEFAULT_IRONSIGHTS_SPEED 200.0f
#define CHARACTER_DEFAULT_SPRINT_SPEED 800.0f
#define CHARACTER_DEFAULT_JUMP_VELOCITY 400.0f
#define CHARACTER_DEFAULT_FOV 90.0f
#define CHARACTER_DEFAULT_IRONSIGHTS_FOV 60.0f

#define COLLISION_PROFILE_CHARACTER "ArenaCharacter"
#define COLLISION_WEAPON ECC_GameTraceChannel1

#define SURFACE_TYPE_DEFAULT SurfaceType_Default
#define SURFACE_TYPE_METAL SurfaceType1
#define SURFACE_TYPE_CONCRETE SurfaceType2
#define SURFACE_TYPE_HEAD SurfaceType3
#define SURFACE_TYPE_BODY SurfaceType4
#define SURFACE_TYPE_FOOT SurfaceType5

UENUM(BlueprintType)
enum class EGameType : uint8
{
	Coop UMETA(DisplayName="Coop"),
	Versus UMETA(DisplayName="Versus"),
	TeamDeathmatch UMETA(DisplayName="TeamDeathmatch")
};