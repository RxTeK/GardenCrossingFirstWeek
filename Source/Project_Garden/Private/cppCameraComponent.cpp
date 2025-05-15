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

	CharaRef = Cast<AMyProject8Character>(GetOwner());

	if (!CharaRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("BP_CameraComponent: CharaRef est NULL !"));
	}
}



// Called every frame
void UcppCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!CharaRef || !CharaRef->GetCameraBoom()) // Vérifie si CharaRef est valide
	{
		return;
	}

	FVector TargetLocation = CharaRef->GetActorLocation();
	FVector CurrentLocation = CharaRef->GetCameraBoom()->GetComponentLocation();
    
	CharaRef->GetCameraBoom()->SetWorldLocation(FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, InterpSpeedlag));

	if(!CharaRef->OnSpline)
	{
		FVector OnSplineCurrentLocation = CharaRef->GetCameraBoom()->GetComponentLocation();
		FVector OnSplineTargetLocation = CharaRef->GetActorLocation();
		float InterpSpeed = 7.0f;

		FVector InterpCameraBoom = FMath::VInterpTo(OnSplineCurrentLocation, OnSplineTargetLocation, DeltaTime, InterpSpeed);
		
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

