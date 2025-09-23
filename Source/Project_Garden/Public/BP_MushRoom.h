#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Materials/Material.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Components/SplineComponent.h"
#include "BP_MushRoom.generated.h"

UCLASS()
class PROJECT_GARDEN_API ABP_MushRoom : public AActor
{
	GENERATED_BODY()
    
public:	
	// Sets default values for this actor's properties
	ABP_MushRoom();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "MushRoomSettings")
	float MushroomPower = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MushRoomSettings")
	float MassForLaunch = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mushroom")
	float LaunchSpeed = 1200.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* Arrow;

	UPROPERTY(VisibleAnywhere)
	UMaterial* Material;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collider;

	UPROPERTY(VisibleAnywhere)
	USplineComponent* TrajectorySpline;
	
	UFUNCTION()
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
};