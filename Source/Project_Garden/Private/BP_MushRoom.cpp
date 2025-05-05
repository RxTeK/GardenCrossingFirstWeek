#include "BP_MushRoom.h"

#include "MyProject8Character.h"

// Sets default values
ABP_MushRoom::ABP_MushRoom()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create and configure the Mesh component
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);

    // Create and configure the BoxComponent
    Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
    Collider->InitBoxExtent(FVector(100.f, 100.f, 100.f)); // Dimensions de la boîte
    Collider->SetCollisionProfileName(TEXT("OverlapAllDynamic")); // Profil de collision
    Collider->SetGenerateOverlapEvents(true); // Activer les événements de superposition
    Collider->SetupAttachment(Mesh); // Attacher à la racine (le Mesh)

    // Bind the overlap event
    if (Collider)
    {
        Collider->OnComponentBeginOverlap.AddDynamic(this, &ABP_MushRoom::OnComponentOverlap);
    }
}

float ABP_MushRoom::randomNumber()
{
    float random = FMath::RandRange(0, 100);
    random = random / 100;
    return random;
}

// Called when the game starts or when spawned
void ABP_MushRoom::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABP_MushRoom::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABP_MushRoom::OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Collision détectée !"));

    if (Mesh && Mesh->GetMaterial(0))
    {
        FName name = FName("Base Color");
        FVector color = FVector(randomNumber(), randomNumber(), randomNumber());
        Mesh->SetVectorParameterValueOnMaterials(name, color);
        UE_LOG(LogTemp, Display, TEXT("Random Color"));
    }
    
    AMyProject8Character* Chararef = Cast<AMyProject8Character>(OtherActor);
    if (Chararef)
    {
        Chararef->LaunchCharacter(Chararef->GetActorUpVector()*1000.0f, true, true);
    }
}