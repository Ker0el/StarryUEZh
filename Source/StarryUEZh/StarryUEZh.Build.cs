// Copyright 星空汉化. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class StarryUEZh : ModuleRules
{
	public StarryUEZh(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp20;
		bWarningsAsErrors = false;

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"ApplicationCore",
			"InputCore",
			"UnrealEd",
			"LevelEditor",
			"ToolMenus",
			"Projects",
			"AssetRegistry",
			"Json",
			"JsonUtilities",
			"HTTP",
			"BlueprintGraph",
			"Kismet",
			"GraphEditor",
			"MaterialEditor",
			"AnimGraph",
			"Niagara",
			"NiagaraEditor"
		});

		// The bundled Detours module only exists from UE 5.8. Older engines link
		// the same Microsoft Detours v4.0.1 static library from ThirdParty.
		if (Target.Version.MajorVersion > 5 || Target.Version.MinorVersion >= 8)
		{
			PrivateDependencyModuleNames.Add("Detours");
		}
		else
		{
			string DetoursDir = Path.Combine(ModuleDirectory, "..", "..", "ThirdParty", "Detours");
			PublicSystemIncludePaths.Add(Path.Combine(DetoursDir, "include"));
			PublicAdditionalLibraries.Add(Path.Combine(DetoursDir, "lib", "detours.lib"));
		}

		PublicSystemLibraries.Add("Advapi32.lib");
	}
}
