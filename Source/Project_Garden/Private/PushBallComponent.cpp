#include "PushBallComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"

UPushBallComponent::UPushBallComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPushBallComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPushBallComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
	if (!PC) return;
	
	FVector Start = OwnerPawn->GetActorLocation();
	FVector End = Start + (OwnerPawn->GetActorForwardVector() * TraceDistance);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerPawn);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Green : FColor::Red, false, 0.0f, 0, 2.0f);

	if (bHit)
	{
		UPrimitiveComponent* HitComp = Hit.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics())
		{
			// Quand le player move
				FVector PushDir = OwnerPawn->GetActorForwardVector();
				HitComp->AddForce(PushDir * PushForce);
		}
	}
}
