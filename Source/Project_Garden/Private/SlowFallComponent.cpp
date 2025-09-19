// Fill out your copyright notice in the Description page of Project Settings.

#include "SlowFallComponent.h"

#include "MyProject8Character.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
USlowFallComponent::USlowFallComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	NewGravity = GravityScaleGlide;

	// ...
}


// Called when the game starts
void USlowFallComponent::BeginPlay()
{
	Super::BeginPlay();

	CharaRef = Cast<AMyProject8Character>(GetOwner());

	if (!CharaRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("BP_CameraComponent: CharaRef est NULL !"));
	}
	
}


// Called every frame
void USlowFallComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(CharaRef->GetCharacterMovement()->IsMovingOnGround())
	{
		DestroyGlider();
		CharaRef->GetCharacterMovement()->GravityScale = GravityScaleClassic;
		CharaRef->GetCharacterMovement()->AirControl = AirControlClassic;
		Plane = true;
		StopPlane = false;
	}
	else if (!Plane) // Donc : en train de planer
	{
		GlideTimer += DeltaTime;
		
		float DeteriorationFactor = FMath::Clamp(GlideTimer / MaxGlideTime, 0.0f, 1.0f);
		
		NewGravity = FMath::Lerp(GravityTemp, GravityScaleClassic, DeteriorationFactor);
		CharaRef->GetCharacterMovement()->GravityScale = NewGravity;
		
		if (GlideTimer >= MaxGlideTime)
		{
			GravityClassic();
			DestroyGlider();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("float: %f"), NewGravity);
}

void USlowFallComponent::SlowFallOn()
{
	if (!AlreadyPlane)
	{
		AlreadyPlane = true;
		if (!CharaRef || !CharaRef->GetCharacterMovement()->IsFalling())
			return;

		// Ne rien faire si l’animation est en cours
		if (CharaRef->bIsStartingGlide)
		{
			return;
		}

		if (Plane && !StopPlane)
		{
			GravityTemp = NewGravity;
			Plane = false;
			CharaRef->GetCharacterMovement()->Velocity.Z = 0;
			CharaRef->GetCharacterMovement()->AirControl = AirControlGlide;
			GlideTimer = 0.0f;
			SpawnAndAttachGlider();
		}
		else
		{
			GravityClassic();
			UE_LOG(LogTemp, Warning, TEXT("PASSSSE"));
		}
	}
}


void USlowFallComponent::GravityClassic()
{
	CharaRef->GetCharacterMovement()->GravityScale = GravityScaleClassic;
	CharaRef->GetCharacterMovement()->AirControl = AirControlClassic;
	Plane = true;
	AlreadyPlane = false;
	DestroyGlider();
}

void USlowFallComponent::SpawnAndAttachGlider()
{
	if (!GliderBPClass || !CharaRef || GliderInstance)
		return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = CharaRef;

	GliderInstance = GetWorld()->SpawnActor<AActor>(GliderBPClass, CharaRef->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);

	if (GliderInstance)
	{
		USkeletalMeshComponent* SkeletalMesh = CharaRef->GetMesh();
		if (SkeletalMesh)
		{
			GliderInstance->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Glider"));
		}
	}
}

void USlowFallComponent::DestroyGlider()
{
	if (GliderInstance)
	{
		GliderInstance->Destroy();
		GliderInstance = nullptr;
	}
}


