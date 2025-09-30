// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABoat::ABoat()
{
	PrimaryActorTick.bCanEverTick = true;
	RootDefault = CreateDefaultSubobject<USceneComponent>(TEXT("Mesh"));
	SetRootComponent(RootDefault);
	
	BoatRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BoatRoot"));
	BoatRoot->SetupAttachment(RootComponent);

	BoatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoatMesh"));
	BoatMesh->SetupAttachment(BoatRoot);

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->InitBoxExtent(FVector(100.f, 100.f, 100.f));
	Collider->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collider->SetGenerateOverlapEvents(true);
	Collider->SetupAttachment(BoatMesh);
	if (Collider)
	{
		Collider->OnComponentBeginOverlap.AddDynamic(this, &ABoat::OnComponentOverlap);
	}
}

// Called when the game starts or when spawned
void ABoat::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABoat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABoat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoat::OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyProject8Character* Chararef = Cast<AMyProject8Character>(OtherActor);
	if (Chararef && DoOnce == false)
	{
		DoOnce = true;
		Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		Controller->Possess(this);
		
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("MoveBoat"));

		GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDel,GetWorld()->GetDeltaSeconds(),true);
		MoveBoat();
	}
}

void ABoat::MoveBoat()
{
	UE_LOG(LogTemp, Warning, TEXT("Reset"));
	
}

