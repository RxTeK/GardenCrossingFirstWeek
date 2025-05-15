// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraSpline.h"

#include "InterfaceCamera.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "InterfaceCamera.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject8Character.h"

// Sets default values
ACameraSpline::ACameraSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);

	Target = CreateDefaultSubobject<USceneComponent>(TEXT("Target"));
	Target->SetupAttachment(RootComponent);

	Enter = CreateDefaultSubobject<UBoxComponent>(TEXT("Enter"));
	Enter->SetupAttachment(RootComponent);
	Enter->SetGenerateOverlapEvents(true);
	Enter->SetCollisionProfileName(TEXT("Trigger"));

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Enter);

	Enter->OnComponentBeginOverlap.AddDynamic(this, &ACameraSpline::OnOverlap);
}

// Called when the game starts or when spawned
void ACameraSpline::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACameraSpline::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyProject8Character* Player = Cast<AMyProject8Character>(OtherActor);
	if (Player && !Player->OnSpline)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UInterfaceCamera::StaticClass()))
		{
			IInterfaceCamera* InterfaceCam = Cast<IInterfaceCamera>(OtherActor);
			if (InterfaceCam)
			{
				InterfaceCam->AddSpline(PlayerFocus, Target, Spline);
			}
		}
	}
}

void ACameraSpline::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

// Called every frame
void ACameraSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
