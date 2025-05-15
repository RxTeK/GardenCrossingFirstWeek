// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/SplineComponent.h"
#include "InterfaceCamera.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterfaceCamera : public UInterface
{
	GENERATED_BODY()
};

class PROJECT_GARDEN_API IInterfaceCamera
{
	GENERATED_BODY()

public:
	
	virtual void AddSpline(bool Player, USceneComponent* Target, USplineComponent* Spline) = 0;
	
	virtual void RemoveSpline() = 0;
};
