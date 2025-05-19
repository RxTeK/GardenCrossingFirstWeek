// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraSpline.h"

#include "BlueprintEditor.h"
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
	Enter->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Enter->SetGenerateOverlapEvents(true);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Enter);

	if(Enter)
	{
		Enter->OnComponentBeginOverlap.AddDynamic(this, &ACameraSpline::OnOverlap);
		Enter->OnComponentBeginOverlap.AddDynamic(this, &ACameraSpline::EndOverlap);
	}
}

// Called when the game starts or when spawned
void ACameraSpline::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACameraSpline::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyProject8Character* Player = Cast<AMyProject8Character>(OtherActor);
	if (Player && !Player->OnSpline)
	{
		TArray<UActorComponent*> Components = OtherActor->GetComponents().Array();
		for (UActorComponent* Component : Components)
		{
			if (Component->GetClass()->ImplementsInterface(UInterfaceCamera::StaticClass()))
			{
				IInterfaceCamera* InterfaceCam = Cast<IInterfaceCamera>(Component);
				if (InterfaceCam)
				{
					UE_LOG(LogTemp, Warning, TEXT("OVERLAP via component"));
					InterfaceCam->AddSpline(PlayerFocus, Target, Spline);
					break;
				}
			}
		}
	}
}

void ACameraSpline::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyProject8Character* Player = Cast<AMyProject8Character>(OtherActor);
	if (Player)
	{
		FVector Vec1 = CharaRef->GetCapsuleComponent()->GetForwardVector();
		FVector Vec2 = Arrow->GetForwardVector();

		if (!Vec1.Equals(Vec2, 0.5f))
		{
			TArray<UActorComponent*> Components = OtherActor->GetComponents().Array();
			for (UActorComponent* Component : Components)
			{
				if (Component->GetClass()->ImplementsInterface(UInterfaceCamera::StaticClass()))
				{
					IInterfaceCamera* InterfaceCam = Cast<IInterfaceCamera>(Component);
					if (InterfaceCam)
					{
						UE_LOG(LogTemp, Warning, TEXT("OVERLAP via component"));
						InterfaceCam->RemoveSpline();
						break;
					}
				}
			}
		}
	}
}

// Called every frame
void ACameraSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
