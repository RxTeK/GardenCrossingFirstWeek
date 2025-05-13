// Fill out your copyright notice in the Description page of Project Settings.


#include "Spline.h"

#include "Misc/MapErrors.h"

// Sets default values
ASpline::ASpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SetRootComponent(SkeletalMesh);

}

// Called when the game starts or when spawned
void ASpline::BeginPlay()
{
	Super::BeginPlay();
	SkeletalMesh->SetAllBodiesBelowSimulatePhysics(FName(TEXT("Bone_118")), true, true);
	
}

// Called every frame
void ASpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

