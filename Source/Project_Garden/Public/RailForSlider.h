#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/BoxComponent.h"
#include "RailForSlider.generated.h"

class AMyProject8Character;
UCLASS()
class PROJECT_GARDEN_API ARailForSlider : public AActor
{
	GENERATED_BODY()

public:
	ARailForSlider();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;

public:
	/** Spline */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USplineComponent* SplineComponent;

	/** Box attachée à la spline */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY()
	AMyProject8Character* PlayerRef;

	UPROPERTY()
	float Distance = 0;

	UPROPERTY()
	float Sign = 0;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DetachPlayer();

	bool OnSpline;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rail")
	UStaticMesh* MeshToUse;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rail")
	float StepDistance = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rail")
	float SpeedOnSpline = 600.f;

	
private:
	void GenerateMeshes();
	void BoxMovement(float DeltaTime);
	void MovementOnSpline();
};
