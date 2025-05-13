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
		CharaRef->GetCharacterMovement()->GravityScale = GravityScaleClassic;
		CharaRef->GetCharacterMovement()->AirControl = AirControlClassic;
		Plane = true;
	}
}

void USlowFallComponent::SlowFallOn()
{
	if (!CharaRef || !CharaRef->GetCharacterMovement()->IsFalling())
		return;

	// Ne rien faire si l’animation est en cours
	if (CharaRef->bIsMontagePlaying)
	{
		UE_LOG(LogTemp, Warning, TEXT("Animation en cours, switch de mode désactivé."));
		return;
	}

	if (Plane)
	{
		GravityClassic();
		Plane = false;
		CharaRef->GetCharacterMovement()->Velocity.Z = 0;
		CharaRef->GetCharacterMovement()->GravityScale = GravityScaleGlide;
		CharaRef->GetCharacterMovement()->AirControl = AirControlGlide;

		CharaRef->PlayMontage();
	}
	else
	{
		GravityClassic();
	}
}


void USlowFallComponent::GravityClassic()
{
	CharaRef->GetCharacterMovement()->GravityScale = GravityScaleClassic;
	CharaRef->GetCharacterMovement()->AirControl = AirControlClassic;
	Plane = true;
}

