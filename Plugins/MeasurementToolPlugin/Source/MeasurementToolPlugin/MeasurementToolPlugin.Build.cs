using UnrealBuildTool;

public class MeasurementToolPlugin : ModuleRules
{
    public MeasurementToolPlugin(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "MeasurementToolPlugin/Public" });
        
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Slate",
            "SlateCore",
            "EditorStyle",
            "UnrealEd",
            "LevelEditor",
            "ToolMenus"
        });

        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.Add("UnrealEd");
        }
    }
}
