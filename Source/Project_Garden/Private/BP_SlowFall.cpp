// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_SlowFall.h"

// Sets default values for this component's properties
UBP_SlowFall::UBP_SlowFall()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBP_SlowFall::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBP_SlowFall::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

