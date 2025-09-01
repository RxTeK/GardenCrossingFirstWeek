// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbingZone.h"

#include "MovieSceneSequenceID.h"
#include "MyProject8Character.h"

// Sets default values
AClimbingZone::AClimbingZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	

}

// Called when the game starts or when spawned
void AClimbingZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClimbingZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
