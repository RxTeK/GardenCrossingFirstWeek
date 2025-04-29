#include "BP_CameraComponent.h"
#include "MyProject8Character.h"
#include "GameFramework/SpringArmComponent.h"

UBP_CameraComponent::UBP_CameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBP_CameraComponent::BeginPlay()
{
	Super::BeginPlay();

	CharaRef = Cast<AMyProject8Character>(GetOwner());

	if (!CharaRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("BP_CameraComponent: CharaRef est NULL !"));
	}
}

void UBP_CameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!CharaRef || !CharaRef->GetCameraBoom()) // Vérifie si CharaRef est valide
	{
		return;
	}

	FVector TargetLocation = CharaRef->GetActorLocation();
	FVector CurrentLocation = CharaRef->GetCameraBoom()->GetComponentLocation();
    
	CharaRef->GetCameraBoom()->SetWorldLocation(FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 7.0f));
}
