// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbingComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UClimbingComponent::UClimbingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UClimbingComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<AMyProject8Character>(GetOwner());
	if (PlayerRef)
	{
		GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Yellow,"Start Climb Component");
		CollisionParams.bTraceComplex = true;
		CollisionParams.AddIgnoredActor(PlayerRef);
		
	}
	

	
}


// Called every frame
void UClimbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PlayerRef)
	{
		CanClimbLeft();
		CanClimbRight();
		CanClimbUp();
		CanClimbDown();
	}
}

bool UClimbingComponent::Climb()
{
	if (PlayerRef)
	{
		FVector TracePos = PlayerRef->GetActorLocation();
		FVector EndLocation = TracePos + PlayerRef->GetActorForwardVector()*40.0f;
		FHitResult OutHit;
		DrawDebugLine(GetWorld(), TracePos, EndLocation, FColor::Green, false, 0.0f, 0, 1.0f);
		if (GetWorld()->LineTraceSingleByChannel(OutHit, TracePos, EndLocation,ECC_Visibility,CollisionParams))
		{
			if (Cast<AClimbingZone>(OutHit.GetActor()))
			{
				GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Green,"True");
				if (PlayerRef->GetCharacterMovement()->MovementMode != MOVE_Flying){PlayerRef->GetCharacterMovement()->SetMovementMode(MOVE_Flying);}
				ClimbedRotation = UKismetMathLibrary::MakeRotFromX(OutHit.Normal).Yaw + 180.0f;
				PlayerRef->SetActorRotation(FRotator(PlayerRef->GetActorRotation().Pitch,ClimbedRotation,PlayerRef->GetActorRotation().Roll));
				return true;
			}
		}
		if (PlayerRef->GetCharacterMovement()->MovementMode == MOVE_Flying){PlayerRef->GetCharacterMovement()->SetMovementMode(MOVE_Walking);}
	}
	GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Green,"False");
	return false;
}

bool UClimbingComponent::CanClimbLeft()
{
	if (PlayerRef)
	{
		FVector LocalOffset = FVector(0.0f, -20.0f, 0.0f);
		FVector TracePos = PlayerRef->GetActorTransform().TransformPosition(LocalOffset);
		FVector EndLocation = TracePos + PlayerRef->GetActorForwardVector()*40.0f;
		FHitResult OutHit;
		DrawDebugLine(GetWorld(), TracePos, EndLocation, FColor::Red, false, 0.0f, 0, 1.0f);
		if (GetWorld()->LineTraceSingleByChannel(OutHit, TracePos, EndLocation,ECC_Visibility,CollisionParams))
		{
			if (Cast<AClimbingZone>(OutHit.GetActor()))
			{
				GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Green,"True");
				return true;
			}
		}
		
	}
	GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Green,"False");
	return false;
}

bool UClimbingComponent::CanClimbRight()
{
	if (PlayerRef)
	{
		FVector LocalOffset = FVector(0.0f, 20.0f, 0.0f);
		FVector TracePos = PlayerRef->GetActorTransform().TransformPosition(LocalOffset);		FVector EndLocation = TracePos + PlayerRef->GetActorForwardVector()*40.0f;
		FHitResult OutHit;
		DrawDebugLine(GetWorld(), TracePos, EndLocation, FColor::Red, false, 0.0f, 0, 1.0f);
		if (GetWorld()->LineTraceSingleByChannel(OutHit, TracePos, EndLocation,ECC_Visibility,CollisionParams))
		{
			if (Cast<AClimbingZone>(OutHit.GetActor()))
			{
				GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Green,"True");
				return true;
			}
		}
		
	}
	GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Green,"False");
	return false;
}

bool UClimbingComponent::CanClimbUp()
{
	if (PlayerRef)
	{
		FVector LocalOffset = FVector(0.0f, 0.0f, 20.0f);
		FVector TracePos = PlayerRef->GetActorTransform().TransformPosition(LocalOffset);
		FVector EndLocation = TracePos + PlayerRef->GetActorForwardVector()*40.0f;
		FHitResult OutHit;
		DrawDebugLine(GetWorld(), TracePos, EndLocation, FColor::Red, false, 0.0f, 0, 1.0f);
		if (GetWorld()->LineTraceSingleByChannel(OutHit, TracePos, EndLocation,ECC_Visibility,CollisionParams))
		{
			if (Cast<AClimbingZone>(OutHit.GetActor()))
			{
				GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Green,"True");
				return true;
			}
		}
		
	}
	GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Green,"False");
	return false;
}

bool UClimbingComponent::CanClimbDown()
{
	if (PlayerRef)
	{
		FVector LocalOffset = FVector(0.0f, 0.0f, -30.0f);
		FVector TracePos = PlayerRef->GetActorTransform().TransformPosition(LocalOffset);
		FVector EndLocation = TracePos + PlayerRef->GetActorForwardVector()*40.0f;
		FHitResult OutHit;
		DrawDebugLine(GetWorld(), TracePos, EndLocation, FColor::Red, false, 0.0f, 0, 1.0f);
		if (GetWorld()->LineTraceSingleByChannel(OutHit, TracePos, EndLocation,ECC_Visibility,CollisionParams))
		{
			if (Cast<AClimbingZone>(OutHit.GetActor()))
			{
				GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Green,"True");
				return true;
			}
		}
		
	}
	GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Green,"False");
	return false;
}

