#include "MeasurementToolActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AMeasurementToolActor::AMeasurementToolActor()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        Mesh->SetStaticMesh(CubeMesh.Object);
    }
}

void AMeasurementToolActor::BeginPlay()
{
    Super::BeginPlay();
}

void AMeasurementToolActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
