// Fill out your copyright notice in the Description page of Project Settings.


#include "cppCameraComponent.h"

#include "LandscapeRender.h"
#include "Kismet/KismetMathLibrary.h"
#include "MaterialHLSLTree.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "MyProject8Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Elements/Framework/TypedElementQueryBuilder.h"
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
	
	if(!CharaRef->OnSpline)
	{
		FVector OnSplineCurrentLocation = CharaRef->GetCameraBoom()->GetComponentLocation();
		FVector OnSplineTargetLocation = CharaRef->GetActorLocation();
		float InterpSpeed = 7.0f;

		FVector InterpCameraBoom = FMath::VInterpTo(OnSplineCurrentLocation, OnSplineTargetLocation, DeltaTime, InterpSpeedlag);
		
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

void UcppCameraComponent::RotationToTarget()
{
	if (!PostRoot || !CharaRef || !CharaRef->GetFollowCamera()) return;

	FVector StartLocation = PlayerGood
		? CharaRef->GetFollowCamera()->GetComponentLocation()
		: CharaRef->GetCapsuleComponent()->GetComponentLocation();

	FVector TargetLocation = PostRoot->GetComponentLocation();
	
	FRotator TargetRotator = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
	
	FRotator CurrentRotator = CharaRef->GetFollowCamera()->GetComponentRotation();

	FRotator InterpRotation = FMath::RInterpTo(CurrentRotator, TargetRotator, GetWorld()->GetDeltaSeconds(), 5.0f);

	CharaRef->GetFollowCamera()->SetWorldRotation(InterpRotation);
}
 

void UcppCameraComponent::AddSpline(bool PLayer, USceneComponent* Target, USplineComponent* Spline)
{
	UE_LOG(LogTemp, Warning, TEXT("AddSpline called !"));
	PlayerGood = PLayer;
	SplineComponent = Spline;
	PostRoot = Target;
	CharaRef->OnSpline = true;

	GetWorld()->GetTimerManager().ClearTimer(EndTimerHandle);
	float Interval = GetWorld()->GetDeltaSeconds();

	GetWorld()->GetTimerManager().SetTimer(StartSplineTimerHandle, this, &UcppCameraComponent::RotationToTarget, 1.0f, false);
}

void UcppCameraComponent::RemoveSpline()
{
	CharaRef->OnSpline = false;
	GetWorld()->GetTimerManager().ClearTimer(StartSplineTimerHandle);
	float Interval = GetWorld()->GetDeltaSeconds();

	GetWorld()->GetTimerManager().SetTimer(EndTimerHandle, this, &UcppCameraComponent::RotationToTarget, 1.0f, false);
	
}

