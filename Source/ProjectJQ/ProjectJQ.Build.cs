// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectJQ : ModuleRules
{
	public ProjectJQ(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivatePCHHeaderFile = "ProjectJQ.h";

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "SlateCore", "Slate" });
    }
}
