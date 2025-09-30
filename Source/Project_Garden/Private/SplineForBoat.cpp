// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineForBoat.h"

// Sets default values
ASplineForBoat::ASplineForBoat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SplineComponent->CreateDefaultSubobject<USceneComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void ASplineForBoat::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASplineForBoat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

