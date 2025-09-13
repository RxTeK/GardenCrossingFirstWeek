// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRiddle.h"
#include "MyGateForRiddle.h"
#include "Misc/MapErrors.h"

// Sets default values
AMyRiddle::AMyRiddle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(StaticMesh);
	StaticMesh->SetGenerateOverlapEvents(true);


}

void AMyRiddle::Touched()
{
	if (MyGateForRiddle)
	{
		MyGateForRiddle->RemoveIsTouched(this);
		K2_DestroyActor();
	}
}

void AMyRiddle::AddGate(AMyGateForRiddle* GateForRiddle)
{
	MyGateForRiddle = GateForRiddle;
}

// Called when the game starts or when spawned
void AMyRiddle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyRiddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

