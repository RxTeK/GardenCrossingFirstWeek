// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraSpline.generated.h"

class USplineComponent;
class UBoxComponent;
class UArrowComponent;
class USceneComponent;

class AMyProject8Character;

UCLASS()
class PROJECT_GARDEN_API ACameraSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraSpline();

	UPROPERTY(EditAnywhere, Blueprintable)
	bool PlayerFocus;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	USplineComponent* Spline;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Target;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Enter;

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* Arrow;

	UPROPERTY()
	TObjectPtr<AMyProject8Character> CharaRef;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
