// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InterfaceCamera.h"
#include "Components/ActorComponent.h"
#include "cppCameraComponent.generated.h"

class AMyProject8Character;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GARDEN_API UcppCameraComponent : public UActorComponent, public IInterfaceCamera
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UcppCameraComponent();

	UPROPERTY(EditAnywhere, Blueprintable)
	float InterpSpeedlag;

	UFUNCTION()
	virtual void AddSpline(bool Player, USceneComponent* Target, USplineComponent* Spline) override;

	UFUNCTION()
	virtual void RemoveSpline() override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TObjectPtr<AMyProject8Character> CharaRef;

	UFUNCTION()
	void ResetPosition();

	UFUNCTION()
	void RotationToTarget();

	UPROPERTY()
	FVector Postition000 = FVector(0, 50, 30);

	UPROPERTY()
	FTimerHandle EndTimerHandle;

	UPROPERTY()
	FTimerHandle StartSplineTimerHandle;

	UPROPERTY()
	USplineComponent* SplineComponent;

	UPROPERTY()
	USceneComponent* PostRoot;

	UPROPERTY()
	bool PlayerGood;
};
