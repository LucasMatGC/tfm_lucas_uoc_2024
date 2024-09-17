// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RogueLike_Project : ModuleRules
{
	public RogueLike_Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
