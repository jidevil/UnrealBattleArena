// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealBattleArena : ModuleRules
{
	public UnrealBattleArena(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore", 
				"PhysicsCore",
				"NavigationSystem",
				"AIModule",
				"GameplayTasks", 
				"OnlineSubsystem",
				"OnlineSubsystemUtils"
			}
		);

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
