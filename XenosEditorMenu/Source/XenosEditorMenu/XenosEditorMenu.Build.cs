// Copyright (c) Ignasi Pelayo, 2023. All Rights Reserved.

using UnrealBuildTool;

public class XenosEditorMenu : ModuleRules
{
	public XenosEditorMenu(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"ToolMenus",
			"EditorStyle"
		});
	}
}
