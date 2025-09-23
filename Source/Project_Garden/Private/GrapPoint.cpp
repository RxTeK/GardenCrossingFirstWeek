// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapPoint.h"
#include "Components/WidgetComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GrapPointWidget.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AGrapPoint::AGrapPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));

	SetRootComponent(Box);
	Widget->SetupAttachment(Box);
	Arrow->SetupAttachment(Box);
	
}



// Called when the game starts or when spawned
void AGrapPoint::BeginPlay()
{
	Super::BeginPlay();
	if (Widget)
	{
		if (UUserWidget* UserWidget = Widget->GetUserWidgetObject())
		{
			GrapPointWidget = Cast<UGrapPointWidget>(UserWidget);
			if (GrapPointWidget)
			{
				CanGrap(false);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "GrapPointWidget");
			}
		}
	}
}

// Called every frame
void AGrapPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetWorld()->GetFirstPlayerController()->GetPawn() != nullptr)
	{
		Widget->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()));
		Arrow->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()));

	}
}

void AGrapPoint::CanGrap(bool GrapOrNot)
{
	if (Widget != nullptr)
	{
		if (GrapPointWidget != nullptr && GrapPointWidget->GrapPointImage != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Red,"Grap Point Widget True");
			GrapPointWidget->GrapPointImage->SetBrush(GrapOrNot ? GrabbedImage : CantGrabImage);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Red,"Grap Point Widget False");

		} 
	}
}