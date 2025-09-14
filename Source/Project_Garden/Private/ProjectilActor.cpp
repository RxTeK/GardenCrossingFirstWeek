// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectilActor.h"
#include "MyRiddle.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectilActor::AProjectilActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 1.5f;
	ProjectileMovement->bSweepCollision = true;

	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComp->SetupAttachment(MeshComp);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionObjectType(ECC_WorldDynamic);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectilActor::OnComponentOverlap);


	
	
}

// Called when the game starts or when spawned
void AProjectilActor::BeginPlay()
{
	Super::BeginPlay();

}

void AProjectilActor::MoveProjectile(float Speed,FRotator Rotation)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = Speed;
		ProjectileMovement->MaxSpeed = Speed;
		ProjectileMovement->Velocity = Rotation.Vector() * ProjectileMovement->InitialSpeed;

	}
}

void AProjectilActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (MeshComp && BoxComp)
	{
		FVector Origin;
		FVector BoxExtent;
		MeshComp->GetLocalBounds(Origin, BoxExtent);
		BoxExtent += FVector(0.5f, 0.5f, 0.5f);
		BoxComp->SetBoxExtent(BoxExtent);
	}
}


void AProjectilActor::OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this->GetOwner() && OtherActor != this)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "ProjectileMovement");
		if (AMyRiddle* MyRiddle = Cast<AMyRiddle>(OtherActor))
		{
			MyRiddle->Touched();
		}
		K2_DestroyActor();
	}
	else
	{
		
	}
}


// Called every frame
void AProjectilActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->GetVelocity() == FVector::ZeroVector)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Destroyed");
		K2_DestroyActor();
	}
}

