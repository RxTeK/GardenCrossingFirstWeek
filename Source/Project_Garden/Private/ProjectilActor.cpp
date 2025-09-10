// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectilActor.h"

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

// Called every frame
void AProjectilActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

