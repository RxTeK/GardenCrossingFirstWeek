// Fill out your copyright notice in the Description page of Project Settings.


#include "cppCameraComponent.h"

#include "MaterialHLSLTree.h"
#include "MyProject8Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
UcppCameraComponent::UcppCameraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UcppCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UcppCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!CharaRef->OnSpline)
	{
		FVector CurrentLocation = CharaRef->GetCameraBoom()->GetComponentLocation();
		FVector TargetLocation = CharaRef->GetActorLocation();
		float InterpSpeed = 7.0f;

		FVector InterpCameraBoom = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, InterpSpeed);
		
		CharaRef->GetCameraBoom()->SetWorldLocation(InterpCameraBoom);
	}
}

void UcppCameraComponent::ResetPostition()
{
	FVector CurrentLocation = CharaRef->GetFollowCamera()->GetRelativeLocation();
	FVector TargetLocation = Postition000;
	float InterpSpeed = 4.0f;

	FVector InterpFollowCamera = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
	
	CharaRef->GetFollowCamera()->SetWorldLocation(InterpFollowCamera);

	FRotator CurrentRotator = CharaRef->GetFollowCamera()->GetRelativeRotation();
	FRotator TargetRotator = CharaRef->GetCameraBoom()->GetRelativeRotation();
	float InterpSpeedrotation = 5.0f;

	FRotator InterprotationCamera = FMath::RInterpTo(CurrentRotator, TargetRotator, GetWorld()->GetDeltaSeconds(), InterpSpeedrotation);	

	CharaRef->GetFollowCamera()->SetWorldRotation(InterprotationCamera);
}

