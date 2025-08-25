// Fill out your copyright notice in the Description page of Project Settings.


#include "SwimComponent.h"

#include "KismetTraceUtils.h"
#include "Math/Vector.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
USwimComponent::USwimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	LenghtOfGrab = 0.0f;
	PickA = false;
	Grabbed = false;
	GravityScaleBase = 0.0f;
	PlayerRef = nullptr;
}


// Called when the game starts
void USwimComponent::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerRef = Cast<AMyProject8Character>(GetOwner());
	if (PlayerRef)
	{
		GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Yellow,"Start Swim Component");
	}
}

void USwimComponent::GrabStart()
{
	GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Yellow,"Start");
	if (PlayerRef->BestGrapPoint != nullptr)
	{
		PlayerRef->bAttached = true;
		GravityScaleBase = PlayerRef->GetCharacterMovement()->GravityScale;
		PlayerRef->CableComponentRef->SetHiddenInGame(false);
		GetGrabStartLocation = PlayerRef->BestGrapPoint->GetActorLocation();
		PlayerRef->CableComponentRef->SetWorldLocation(GetGrabStartLocation);
		PlayerRef->CableComponentRef->EndLocation = PlayerRef->GetMesh()->GetSocketTransform(FName("hand_r"),ERelativeTransformSpace::RTS_Actor).GetLocation();
		LenghtOfGrab = ((PlayerRef->GetActorLocation() - GetGrabStartLocation).Length()) - 400.0f;
		PlayerRef->CableComponentRef->CableLength = LenghtOfGrab;
		Grabbed = true;
	}
}

void USwimComponent::GrabEnd()
{
	if (Grabbed && PlayerRef)
	{
		PlayerRef->GetCharacterMovement()->GravityScale = GravityScaleBase;
		if (!PlayerRef->GetCharacterMovement()->IsMovingOnGround())
		{
			PlayerRef->LaunchCharacter(PlayerRef->GetVelocity().GetSafeNormal(1E-6) * 500,false,false);
		}
		Grabbed = false;
		PlayerRef->CableComponentRef->SetHiddenInGame(true);
		PlayerRef->GetCharacterMovement()->AirControl = 0.35f;
		PlayerRef->bAttached = false;
	}

}

void USwimComponent::CalculateSwingForce()
{
	if (Grabbed && PlayerRef != nullptr && PlayerRef->BestGrapPoint != nullptr)
	{
		if (!IsMovingOnGround())
		{
			PlayerRef->CableComponentRef->SetWorldLocation(GetGrabStartLocation);
			PlayerRef->CableComponentRef->EndLocation = PlayerRef->GetMesh()->GetSocketTransform(FName("hand_r"),ERelativeTransformSpace::RTS_Actor).GetLocation();
			const float Value = ((PlayerRef->GetActorLocation().Z + 500.0f) > PlayerRef->BestGrapPoint->GetActorLocation().Z) ? 1.5f : 0.0f;
			const float Force = FMath::Abs(Value * 2.0f);
			PlayerRef->GetCharacterMovement()->GravityScale = Force;
			PlayerRef->GetCharacterMovement()->AirControl = 2.0f;
			
			float MyValue = PlayerRef->MovementVector.Length();
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Yellow,FString::SanitizeFloat(MyValue));
			if (PlayerRef->MovementVector.Length() <= 0.1f && PlayerRef->MovementVector.Length() >= -0.1f)
			{
				FVector NewDirection = (PlayerRef->BestGrapPoint->GetActorLocation() - PlayerRef->GetActorLocation()).GetSafeNormal(1E-08) * 5.0f;
				PlayerRef->GetCharacterMovement()->AddInputVector(FVector(NewDirection.X, NewDirection.Y, 0.0f));
			}
			
			FVector DiffPlayerAndPoint = PlayerRef->GetActorLocation()-GetGrabStartLocation;
			FVector Direction = (DiffPlayerAndPoint.GetSafeNormal(1E-8) * (FVector::DotProduct(PlayerRef->GetVelocity(),DiffPlayerAndPoint)))* -5.0f;
			
			PlayerRef->GetCharacterMovement()->AddForce(Direction);
		}
		else
		{
			PlayerRef->CableComponentRef->SetWorldLocation(GetGrabStartLocation);
			PlayerRef->CableComponentRef->EndLocation = PlayerRef->GetMesh()->GetSocketTransform(FName("hand_r"),ERelativeTransformSpace::RTS_Actor).GetLocation();
			float NewCableRange = (PlayerRef->GetActorLocation() - GetGrabStartLocation).Length() - 400.0f;
			PlayerRef->CableComponentRef->CableLength = NewCableRange;
		}
	}
	else
	{
		GrabEnd();
	}
}

bool USwimComponent::IsMovingOnGround()
{
	FHitResult Hit;
	const ECollisionChannel TraceChanel = ECC_Visibility;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PlayerRef);
	QueryParams.bTraceComplex = false;
	QueryParams.bReturnPhysicalMaterial = false;
	
	float CapsuleRadius = PickA ? 150.0f : 25.0f;
	float CapsuleHalfHeight = 25.0f;
	const FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
	bool bHit = GetWorld()->SweepSingleByChannel(Hit,PlayerRef->GetActorLocation(),PlayerRef->GetActorLocation() + (PlayerRef->GetActorUpVector() * -200.0f), FQuat::Identity, TraceChanel, CapsuleShape, QueryParams);
	PickA = bHit;
	return bHit;
}

// Called every frame
void USwimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CalculateSwingForce();
}




