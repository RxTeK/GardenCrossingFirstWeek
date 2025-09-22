#include "BP_MushRoom.h"

#include "MyProject8Character.h"
#include "SlowFallComponent.h"

// Sets default values
ABP_MushRoom::ABP_MushRoom()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create and configure the Mesh component
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);
    
    Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
    Arrow->SetupAttachment(Mesh);
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
{w
    AMyProject8Character* Chararef = Cast<AMyProject8Character>(OtherActor);
    if (Chararef)
    {
        if(USlowFallComponent* comp = Chararef->SlowFallComponent)
        {
            comp->GravityClassic();
        }
        Chararef->LaunchCharacter(Arrow->GetForwardVector() * m_MushroomPower, true, true);
    }
}