// Fill out your copyright notice in the Description page of Project Settings.


#include "cppCameraComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "MyProject8Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

		FVector InterpCameraBoom = FMath::VInterpTo(OnSplineCurrentLocation, OnSplineTargetLocation, DeltaTime, InterpSpeedlag);
		
		CharaRef->GetCameraBoom()->SetWorldLocation(InterpCameraBoom);
	}
}

void UcppCameraComponent::ResetPosition()
{
	FVector CurrentLocation = CharaRef->GetFollowCamera()->GetRelativeLocation();
	FVector TargetLocation = Postition000;
	float InterpSpeed = 4.0f;

	FVector InterpFollowCamera = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
	
	CharaRef->GetFollowCamera()->SetRelativeLocation(InterpFollowCamera);

	FRotator CurrentRotator = CharaRef->GetFollowCamera()->GetRelativeRotation();
	FRotator TargetRotator = CharaRef->GetCameraBoom()->GetRelativeRotation();
	float InterpSpeedrotation = 5.0f;

	FRotator InterprotationCamera = FMath::RInterpTo(CurrentRotator, TargetRotator, GetWorld()->GetDeltaSeconds(), InterpSpeedrotation);	

	CharaRef->GetFollowCamera()->SetRelativeRotation(InterprotationCamera);
}

void UcppCameraComponent::RotationToTarget()
{
	if (!PostRoot || !CharaRef || !CharaRef->GetFollowCamera()) return;
	{
		FVector StartLocation = PlayerGood
			? CharaRef->GetCapsuleComponent()->GetComponentLocation()
			: PostRoot->GetComponentLocation();

		FVector TargetLocationRotation = CharaRef.Get()->GetFollowCamera()->GetComponentLocation();
		FRotator TargetRotator = UKismetMathLibrary::FindLookAtRotation(TargetLocationRotation, StartLocation);
		FRotator CurrentRotator = CharaRef->GetFollowCamera()->GetComponentRotation();
		FRotator InterpRotation = FMath::RInterpTo(CurrentRotator, TargetRotator, GetWorld()->GetDeltaSeconds(), 5.0f);

		CharaRef->GetFollowCamera()->SetWorldRotation(InterpRotation);

		FVector CurrentLocation = CharaRef->GetFollowCamera()->GetComponentLocation();
		
		FVector TargetLocation = SplineComponent->FindLocationClosestToWorldLocation(CharaRef->GetActorLocation(), ESplineCoordinateSpace::World);
		FVector InterpLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), 4.0f);

		CharaRef->GetFollowCamera()->SetWorldLocation(InterpLocation);
		
	}
}
 

void UcppCameraComponent::AddSpline(bool PLayer, USceneComponent* Target, USplineComponent* Spline)
{
	PlayerGood = PLayer;
	SplineComponent = Spline;
	PostRoot = Target;
	CharaRef->OnSpline = true;

	GetWorld()->GetTimerManager().ClearTimer(EndTimerHandle);
	float Interval = GetWorld()->GetDeltaSeconds();
	GetWorld()->GetTimerManager().SetTimer(StartSplineTimerHandle, this, &UcppCameraComponent::RotationToTarget, Interval, true);
}

void UcppCameraComponent::RemoveSpline()
{
	CharaRef->OnSpline = false;
	GetWorld()->GetTimerManager().ClearTimer(StartSplineTimerHandle);
	float Interval = GetWorld()->GetDeltaSeconds();
	GetWorld()->GetTimerManager().SetTimer(EndTimerHandle, this, &UcppCameraComponent::ResetPosition, Interval, true);
}

