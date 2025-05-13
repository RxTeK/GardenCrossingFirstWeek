#include "MeasurementToolPlugin.h"
#include "RulerTool.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "Editor/EditorEngine.h"
#include "Editor/UnrealEd/Public/EditorModeManager.h"

#define LOCTEXT_NAMESPACE "FMeasurementToolPluginModule"

// Définition du plugin
void FMeasurementToolPluginModule::StartupModule()
{
    // Enregistrement du menu et ajout du bouton
    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateLambda([]()
    {
        // Extension du menu principal de l'éditeur
        UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
        FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");

        // Ajout d'une entrée pour le bouton "Activer l’Outil de Mesure"
        Section.AddMenuEntry(
            "MeasurementToolPlugin.SpawnTool",
            FText::FromString("Activer l Outil de Mesure"),
            FText::FromString("Fait apparaître l’outil de mesure dans la scène"),
            FSlateIcon(),
            FUIAction(FExecuteAction::CreateLambda([]()
            {
                // Récupérer l'instance du monde
                UWorld* World = GEditor->GetEditorWorldContext().World();
                if (World)
                {
                    // Spawner l'outil de mesure dans la scène à la position (0,0,0)
                    World->SpawnActor<ARulerTool>(ARulerTool::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
                }
            }))
        );
    }));
}

void FMeasurementToolPluginModule::ShutdownModule()
{
    // Actions à effectuer lors de l'arrêt du module (facultatif)
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMeasurementToolPluginModule, MeasurementToolPlugin)
