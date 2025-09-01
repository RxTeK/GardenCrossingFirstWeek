// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject8Character.h"
#include "ClimbingZone.h"
#include "Components/ActorComponent.h"
#include "ClimbingComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GARDEN_API UClimbingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UClimbingComponent();

	UPROPERTY(EditAnywhere)
	AMyProject8Character* PlayerRef;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	FCollisionQueryParams CollisionParams;
	float ClimbedRotation;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool Climb();
	bool CanClimbLeft();
	bool CanClimbRight();
	bool CanClimbUp();
	bool CanClimbDown();

		
};
