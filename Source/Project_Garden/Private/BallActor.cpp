#include "BallActor.h"
#include "Components/StaticMeshComponent.h"

ABallActor::ABallActor()
{
	PrimaryActorTick.bCanEverTick = true;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	RootComponent = BallMesh;

	// Activer la physique
	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetEnableGravity(true);
	BallMesh->SetLinearDamping(0.5f);   // ralentit la boule
	BallMesh->SetAngularDamping(0.8f);  // évite qu’elle roule à l’infini
	BallMesh->SetMassOverrideInKg(NAME_None, 100.f); // boule lourde
}

void ABallActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
