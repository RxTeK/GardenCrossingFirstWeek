// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGateForRiddle.h"

#include "MyRiddle.h"

// Sets default values
AMyGateForRiddle::AMyGateForRiddle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(StaticMesh);

}

// Called when the game starts or when spawned
void AMyGateForRiddle::BeginPlay()
{
	Super::BeginPlay();

	for (AMyRiddle* Riddle : Riddles )
	{
		Riddle->AddGate(this);
	}
	
}

void AMyGateForRiddle::RemoveIsTouched(AMyRiddle* Riddle)
{
	if (Riddles.Find(Riddle) != -1)
	{
		Riddles.Remove(Riddle);
		if (Riddles.Num() == 0)
		{
			K2_DestroyActor();
		}
	}
}

// Called every frame
void AMyGateForRiddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

