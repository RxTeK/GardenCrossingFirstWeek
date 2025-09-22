#include "RailForSlider.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject8Character.h"
#include "VectorTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

ARailForSlider::ARailForSlider()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Spline
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComponent->SetupAttachment(RootComponent);

	// Box
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComponent->SetupAttachment(SplineComponent);

	// Taille et collision par défaut
	BoxComponent->SetBoxExtent(FVector(32.f, 32.f, 32.f));
BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_WorldDynamic);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ARailForSlider::OnBoxBeginOverlap);
}

void ARailForSlider::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GenerateMeshes();
}

void ARailForSlider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BoxMovement(DeltaTime);
	MovementOnSpline();
}

void ARailForSlider::BeginPlay()
{
	Super::BeginPlay();
}

void ARailForSlider::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									  bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	AMyProject8Character* Player = Cast<AMyProject8Character>(OtherActor);
	if (!Player) return;

	PlayerRef = Player;
	PlayerRef->bAttach = true;

	// Calculer la distance sur la spline **depuis la position actuelle du joueur**
	Distance = SplineComponent->GetDistanceAlongSplineAtLocation(PlayerRef->GetActorLocation(),
																 ESplineCoordinateSpace::World);

	// Déterminer le signe selon la direction du joueur
	FVector PlayerForward = PlayerRef->GetActorForwardVector();
	FVector SplineDir = SplineComponent->GetLocationAtDistanceAlongSpline(Distance + 1, ESplineCoordinateSpace::World)
					   - SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

	Sign = (FVector::DotProduct(PlayerForward, SplineDir) > 0) ? 1.0f : -1.0f;

	// Stopper le mouvement du joueur
	if (PlayerRef->GetCharacterMovement())
	{
		PlayerRef->GetCharacterMovement()->SetMovementMode(MOVE_None);
	}
}


void ARailForSlider::GenerateMeshes()
{
	// Nettoyer d’anciens composants
	TArray<USplineMeshComponent*> OldMeshes;
	GetComponents(OldMeshes);
	for (auto* Comp : OldMeshes)
	{
		Comp->DestroyComponent();
	}

	if (!MeshToUse || !SplineComponent) return;

	const float SplineLength = SplineComponent->GetSplineLength();
	float Distancer = 0.0f;

	while (Distancer < SplineLength)
	{
		const FVector StartPos = SplineComponent->GetLocationAtDistanceAlongSpline(Distancer, ESplineCoordinateSpace::Local);
		const FVector StartTangent = SplineComponent->GetTangentAtDistanceAlongSpline(Distancer, ESplineCoordinateSpace::Local);

		Distancer += StepDistance;

		const FVector EndPos = SplineComponent->GetLocationAtDistanceAlongSpline(Distancer, ESplineCoordinateSpace::Local);
		const FVector EndTangent = SplineComponent->GetTangentAtDistanceAlongSpline(Distancer, ESplineCoordinateSpace::Local);

		USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this);
		SplineMesh->SetCollisionResponseToAllChannels(ECR_Block);
		if (SplineMesh)
		{
			SplineMesh->RegisterComponent();
			SplineMesh->SetStaticMesh(MeshToUse);
			SplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
			SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
		}
	}
}

void ARailForSlider::BoxMovement(float DeltaTime)
{
	ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!PlayerChar) return;

	FVector PlayerLocation = PlayerChar->GetActorLocation();

	float MovementAmount = DeltaTime * SpeedOnSpline;

	FVector NewLocation = PlayerLocation + FVector(MovementAmount, 0.f, 0.f);

	BoxComponent->SetWorldLocation(
		SplineComponent->FindLocationClosestToWorldLocation(NewLocation, ESplineCoordinateSpace::World)
	);
}

void ARailForSlider::MovementOnSpline()
{
	if (!PlayerRef->bAttach && PlayerRef)
	{
		Distance = SplineComponent->GetDistanceAlongSplineAtLocation(BoxComponent->GetComponentLocation(), ESplineCoordinateSpace::World);
		FVector PlayerForward = PlayerRef->GetActorForwardVector();
		FVector SplinePos = SplineComponent->GetLocationAtDistanceAlongSpline(Distance + 1, ESplineCoordinateSpace::World) -
							SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		FVector SplinePosFinal = SplinePos.GetSafeNormal();
		float Dot = FVector::DotProduct(PlayerForward, SplinePosFinal);

		Dot = (Dot > 0) ? Dot + 1.0f : Dot - 1.0f;

		Sign = (Distance >= SplineComponent->GetSplineLength()) ? -1 : Dot;
	}
	else if (PlayerRef)
	{
		FVector VectorPlayer = SplineComponent->GetLocationAtDistanceAlongSpline(Distance + Sign, ESplineCoordinateSpace::World) -SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		FRotator RotatorPlayer = UKismetMathLibrary::MakeRotFromX(VectorPlayer);
		PlayerRef->SetActorLocation(SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World) + FVector (0,0,70));
		PlayerRef->SetActorRotation(RotatorPlayer);
		Distance += Sign * 10.0f;
		PlayerRef->SpeedEffectOn();
		
		if (Distance >= SplineComponent->GetSplineLength() || Distance <= 0.0f)
		{
			DetachPlayer();
		}
	}
}

void ARailForSlider::DetachPlayer()
{
	if (!PlayerRef) return;

	PlayerRef->bAttach = false;
	
	if (PlayerRef->GetCharacterMovement())
	{
		PlayerRef->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	
	FVector DetachImpulse = PlayerRef->GetActorForwardVector() * 1500.0f + FVector(0,0,1000.f);
	PlayerRef->LaunchCharacter(DetachImpulse, true, true);
	PlayerRef->SpeedEffectOff();
}
