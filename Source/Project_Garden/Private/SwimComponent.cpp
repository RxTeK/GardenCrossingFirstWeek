// Fill out your copyright notice in the Description page of Project Settings.


#include "SwimComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
USwimComponent::USwimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	
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
	if (Grabbed)
	{
		PlayerRef->GetCharacterMovement()->GravityScale = GravityScaleBase;
		if (!PlayerRef->GetCharacterMovement()->IsMovingOnGround())
		{
			PlayerRef->LaunchCharacter(PlayerRef->GetVelocity().GetSafeNormal(1E-6) * 500,false,false);
		}
		Grabbed = false;
		PlayerRef->CableComponentRef->SetHiddenInGame(true);
		PlayerRef->GetCharacterMovement()->AirControl = 0.35f;
	}

}

// Called every frame
void USwimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}




