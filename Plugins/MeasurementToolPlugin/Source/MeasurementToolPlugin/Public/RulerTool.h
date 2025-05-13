#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RulerTool.generated.h"

UCLASS()
class MEASUREMENTTOOLPLUGIN_API ARulerTool : public AActor
{
	GENERATED_BODY()
	
public:	
	ARulerTool();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;
	virtual bool ShouldTickIfViewportsOnly() const override { return true; }

	// Met à jour le texte
	void UpdateTextRender(float Distance, const FVector& MidPoint);

public:	
	UPROPERTY(EditAnywhere, Category = "Ruler")
	USceneComponent* StartPoint;

	UPROPERTY(EditAnywhere, Category = "Ruler")
	USceneComponent* EndPoint;

	UPROPERTY(EditAnywhere, Category = "Ruler")
	FColor LineColor = FColor::Red;

	UPROPERTY(EditAnywhere, Category = "Ruler")
	float LineThickness = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Ruler")
	FColor TextColor = FColor::White;

private:
	UPROPERTY()
	class UTextRenderComponent* TextRender;
};
