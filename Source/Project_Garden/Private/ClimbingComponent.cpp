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
				if (PlayerRef->GetCharacterMovement()->MovementMode != MOVE_Flying)
				{
					PlayerRef->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
					PlayerRef->GetCharacterMovement()->MaxFlySpeed = MaxClimbSpeed;
					PlayerRef->GetCharacterMovement()->BrakingDecelerationFlying = MaxClimbSpeed * 10.0f;
				}
				const float ClimbedRotation = UKismetMathLibrary::MakeRotFromX(OutHit.Normal).Yaw + 180.0f;
				PlayerRef->SetActorRotation(FRotator(PlayerRef->GetActorRotation().Pitch,ClimbedRotation,PlayerRef->GetActorRotation().Roll));
				return true;
			}
		}
		if (PlayerRef->GetCharacterMovement()->MovementMode == MOVE_Flying)
		{
			PlayerRef->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			PlayerRef->GetCharacterMovement()->MaxFlySpeed = 500.0f;
			PlayerRef->GetCharacterMovement()->BrakingDecelerationFlying = 0.0f;
		}
	}
	GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Green,"False");
	return false;
}




bool UClimbingComponent::CanClimbLeftOrRight(float Direction)
{
	if (PlayerRef)
	{
		FVector LocalOffset = FVector(0.0f, 20.0f * FMath::Sign(Direction), 0.0f);
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

bool UClimbingComponent::CanClimbUpOrDown(float Direction)
{
	if (PlayerRef)
	{
		FVector LocalOffset = FVector(0.0f, 0.0f, 20.0f * FMath::Sign(Direction));
		FVector TracePos = PlayerRef->GetActorTransform().TransformPosition(LocalOffset);
		FVector EndLocationForward = TracePos + PlayerRef->GetActorForwardVector()*40.0f;
		FVector EndLocationUp = TracePos + PlayerRef->GetActorUpVector()* (40.0f* FMath::Sign(Direction));
		FHitResult OutHitForward;
		FHitResult OutHitUp;
		DrawDebugLine(GetWorld(), TracePos, EndLocationUp, FColor::Red, false, 0.0f, 0, 1.0f);
		if (GetWorld()->LineTraceSingleByChannel(OutHitForward, TracePos, EndLocationForward,ECC_Visibility,CollisionParams))
		{
			if (GetWorld()->LineTraceSingleByChannel(OutHitUp, TracePos, EndLocationUp,ECC_Visibility,CollisionParams) && FMath::Sign(Direction) < 0.0f)
			{
				OnGround = true;
				GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Blue,"True");

			}
			else
			{
				OnGround = false;
				GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Blue,"False");

			}
			
			if (Cast<AClimbingZone>(OutHitForward.GetActor()))
			{
				GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Green,"True");
				return true;
			}
		}
		
	}
	GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Green,"False");
	return false;
}



