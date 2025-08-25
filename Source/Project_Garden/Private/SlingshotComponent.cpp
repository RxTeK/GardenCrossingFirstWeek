// Fill out your copyright notice in the Description page of Project Settings.


#include "SlingshotComponent.h"

// Sets default values for this component's properties
USlingshotComponent::USlingshotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PlayerRef = Cast<AMyProject8Character>(GetOwner());
	
}




// Called when the game starts
void USlingshotComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USlingshotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USlingshotComponent::ShootStart()
{
	GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Blue,"Charge");
	
}

void USlingshotComponent::ShootEnd()
{
	GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Blue,"Shoot");
}
