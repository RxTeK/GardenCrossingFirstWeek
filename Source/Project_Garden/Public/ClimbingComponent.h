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

	UPROPERTY()
	AMyProject8Character* PlayerRef;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	FCollisionQueryParams CollisionParams;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool Climb();
	bool CanClimbLeftOrRight(float Direction);
	bool CanClimbUpOrDown(float Direction);

	UPROPERTY()
	bool OnGround;

	UPROPERTY(EditAnywhere)
	float MaxClimbSpeed;

		
};
