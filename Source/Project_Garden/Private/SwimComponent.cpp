// Fill out your copyright notice in the Description page of Project Settings.


#include "SwimComponent.h"

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
	CharacterRef = Cast<AMyProject8Character>(GetOwner());
	if (CharacterRef)
	{
		GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Yellow,"Start Swim Component");
	}
}

void USwimComponent::GrabStart()
{
	GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Yellow,"Start");

}

void USwimComponent::GrabEnd()
{
	GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Yellow,"END");

}

// Called every frame
void USwimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}




