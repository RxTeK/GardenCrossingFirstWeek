#include "BP_MushRoom.h"
#include "MyProject8Character.h"
#include "SlowFallComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Sets default values
ABP_MushRoom::ABP_MushRoom()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);

    Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
    Arrow->SetupAttachment(Mesh);

    Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
    Collider->InitBoxExtent(FVector(100.f, 100.f, 100.f));
    Collider->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    Collider->SetGenerateOverlapEvents(true);
    Collider->SetupAttachment(Mesh);
    if (Collider)
    {
        Collider->OnComponentBeginOverlap.AddDynamic(this, &ABP_MushRoom::OnComponentOverlap);
    }
    TrajectorySpline = CreateDefaultSubobject<USplineComponent>(TEXT("TrajectorySpline"));
    TrajectorySpline->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ABP_MushRoom::BeginPlay()
{
    Super::BeginPlay();
}

void ABP_MushRoom::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (!TrajectorySpline || !Arrow)
        return;

    // Reset spline
    TrajectorySpline->ClearSplinePoints();

    // Params pour PredictProjectilePath
    FPredictProjectilePathParams PathParams;
    PathParams.StartLocation = Arrow->GetComponentLocation();
    PathParams.LaunchVelocity = Arrow->GetForwardVector() * LaunchSpeed;
    PathParams.ProjectileRadius = 5.f;
    PathParams.MaxSimTime = 5.f;
    PathParams.bTraceWithCollision = true;
    PathParams.SimFrequency = 15.f;
    PathParams.ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
    PathParams.OverrideGravityZ = GetWorld()->GetGravityZ(); // prend en compte gravité du monde

    FPredictProjectilePathResult PathResult;

    // Calcul trajectoire
    bool bHit = UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);
    
    // Ajout des points dans la spline
    for (int32 i = 0; i < PathResult.PathData.Num(); i++)
    {
        const FVector& Pos = PathResult.PathData[i].Location;
        TrajectorySpline->AddSplinePoint(Pos, ESplineCoordinateSpace::World, true);
    }

    TrajectorySpline->UpdateSpline();

    // (optionnel) debug visuel
    for (int32 i = 0; i < PathResult.PathData.Num() - 1; i++)
    {
        DrawDebugLine(GetWorld(), PathResult.PathData[i].Location, PathResult.PathData[i + 1].Location, FColor::Green, false, 0.f, 0, 2.f);
    }
}

// Called every frame
void ABP_MushRoom::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABP_MushRoom::OnComponentOverlap(
    UPrimitiveComponent* OverlappedComp, 
    AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, 
    int32 OtherBodyIndex, 
    bool bFromSweep, 
    const FHitResult& SweepResult)
{
    AMyProject8Character* Chararef = Cast<AMyProject8Character>(OtherActor);
    if (Chararef)
    {
        if (USlowFallComponent* comp = Chararef->SlowFallComponent)
        {
            comp->GravityClassic();
        }

        // Utilise la même valeur que dans PredictProjectilePath
        Chararef->LaunchCharacter(Arrow->GetForwardVector() * LaunchSpeed, true, true);
    }
}