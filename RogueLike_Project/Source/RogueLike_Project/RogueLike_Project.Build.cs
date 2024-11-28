// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RogueLike_Project : ModuleRules
{
	public RogueLike_Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "AIModule" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NavigationSystem" });
	}
}
