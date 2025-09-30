// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MyProject8Character.h"
#include "SplineForBoat.h"
#include "Boat.generated.h"

class ASplineForBoat;

UCLASS()
class PROJECT_GARDEN_API ABoat : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABoat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USceneComponent* RootDefault;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoatRoot;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoatMesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collider;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool DoOnce = false;

private:
	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere)
	ASplineForBoat* SplineBoat;

	UPROPERTY(EditAnywhere)
	float Speed = 200.f;

	UPROPERTY(EditAnywhere)
	float LateralOffset = 0.f;

	UPROPERTY(EditAnywhere)
	float MaxOffset = 100.f;

	float DistanceAlongSpline = 0.f;

	UFUNCTION()
	void MoveBoat();
	

};
