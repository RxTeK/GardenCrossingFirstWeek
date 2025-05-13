#include "RulerTool.h"
#include "DrawDebugHelpers.h"
#include "Components/TextRenderComponent.h"

#if WITH_EDITOR
#include "Editor.h"
#include "EditorViewportClient.h"
#endif

ARulerTool::ARulerTool()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = false;

#if WITH_EDITOR
	bRunConstructionScriptOnDrag = true;
#endif

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	StartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("StartPoint"));
	StartPoint->SetupAttachment(RootComponent);

	EndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("EndPoint"));
	EndPoint->SetupAttachment(RootComponent);

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	TextRender->SetupAttachment(RootComponent);
	TextRender->SetHorizontalAlignment(EHTA_Center);
	TextRender->SetVerticalAlignment(EVRTA_TextCenter);
	TextRender->SetTextRenderColor(FColor::White);
	TextRender->SetText(FText::FromString("0 cm"));
	TextRender->SetWorldSize(50.f);
}

void ARulerTool::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	// Rien ici, on utilise Tick() pour affichage continu
}

void ARulerTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
#if WITH_EDITOR
	if (!GetWorld()) return;

	EWorldType::Type WorldType = GetWorld()->WorldType;
	if (WorldType != EWorldType::Editor && WorldType != EWorldType::EditorPreview)
		return;

	// Calcul distance
	FVector Start = StartPoint->GetComponentLocation();
	FVector End = EndPoint->GetComponentLocation();
	float Distance = FVector::Distance(Start, End);
	FVector MidPoint = (Start + End) / 2;

	// Affiche ligne
	DrawDebugLine(GetWorld(), Start, End, LineColor, false, -1.f, 0, LineThickness);

	// Affiche texte orienté caméra
	UpdateTextRender(Distance, MidPoint);
#endif
}

void ARulerTool::UpdateTextRender(float Distance, const FVector& MidPoint)
{
	FString Str = FString::Printf(TEXT("%.1f cm"), Distance);
	TextRender->SetText(FText::FromString(Str));
	TextRender->SetTextRenderColor(TextColor);

	FVector NewLoc = MidPoint + FVector(0, 0, 50.f);
	TextRender->SetWorldLocation(NewLoc);

#if WITH_EDITOR
	if (GEditor && GEditor->GetActiveViewport())
	{
		if (FEditorViewportClient* VC = static_cast<FEditorViewportClient*>(GEditor->GetActiveViewport()->GetClient()))
		{
			FVector CameraLoc = VC->GetViewLocation();
			FVector Direction = CameraLoc - NewLoc;
			FRotator LookAtRot = FRotationMatrix::MakeFromX(Direction).Rotator();

			LookAtRot.Pitch = 0;
			LookAtRot.Roll = 0;

			TextRender->SetWorldRotation(LookAtRot);
		}
	}
#endif
}
