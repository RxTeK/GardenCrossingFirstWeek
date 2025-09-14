// Fill out your copyright notice in the Description page of Project Settings.

#include "SlingshotComponent.h"

#include "ProjectilActor.h"
#include "MainWidget.h"
#include "MaterialHLSLTree.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values for this component's properties
USlingshotComponent::USlingshotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	
}




// Called when the game starts
void USlingshotComponent::BeginPlay()
{
	Super::BeginPlay();
	PlayerRef = Cast<AMyProject8Character>(GetOwner());
	TickBase = 0.05f;
	
	// ...
	
}


void USlingshotComponent::ArmsForShot(float Lenght)
{
	PlayerRef->GetCameraBoom()->TargetArmLength = FMath::FInterpTo(PlayerRef->GetCameraBoom()->TargetArmLength,Lenght,GetWorld()->DeltaTimeSeconds,7.0f);
}

// Called every frame
void USlingshotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USlingshotComponent::ShootStart()
{
	if (PlayerRef)
	{
		if (ChargePower == MinimalPower)
		{
			GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
			ArmsLenghtBase = PlayerRef->GetCameraBoom()->TargetArmLength;
			In = true;
			PlayerRef->GetWorldTimerManager().SetTimer(ShotTimer, [this](){this->ArmsForShot(TargetArmsLenghtForShot);}, GetWorld()->DeltaTimeSeconds, true);
		}
		
		ChargePower += TickBase * 5.f;
		ChargePower = FMath::Clamp(ChargePower,MinimalPower,MaximalPower);
		FString Msg = FString::Printf(TEXT("La valeur est : %f"), ChargePower);
		FRotator CameraRotation = PlayerRef->GetFollowCamera()->GetComponentRotation();
		PlayerRef->SetActorRotation(FRotator(PlayerRef->GetActorRotation().Pitch, CameraRotation.Yaw, PlayerRef->GetActorRotation().Roll));
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, Msg);
		GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Blue,TEXT("ChargePower"));
		if (PlayerRef->MainWidgetInstance)
		{
			PlayerRef->MainWidgetInstance->StartAiming();
		}
	}
	
	
}

void USlingshotComponent::ShootEnd()
{
	if (ProjectileClass && PlayerRef)
	{
		GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
		PlayerRef->GetWorldTimerManager().SetTimer(ShotTimer, [this](){this->ArmsForShot(ArmsLenghtBase);}, GetWorld()->DeltaTimeSeconds, true);
		GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Blue,"Shoot");
		FVector StartLocation = PlayerRef->GetActorLocation() + (PlayerRef->GetFollowCamera()->GetForwardVector() * 100.f) + (PlayerRef->GetActorRightVector() * 50.f);
		DrawDebugLine(GetWorld(), PlayerRef->GetActorLocation(), StartLocation, FColor::Red, false, 10.0f, 0, 1.0f);
		FRotator ProjectilRotation = PlayerRef->GetFollowCamera()->GetComponentRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.Owner = PlayerRef;

		AProjectilActor* Projectile = GetWorld()->SpawnActor<AProjectilActor>(ProjectileClass,StartLocation,ProjectilRotation,SpawnParams);
		
		
		if (Projectile != nullptr && Projectile->ProjectileMovement )
		{
			Projectile->MoveProjectile(ChargePower*20.0f,ProjectilRotation);
		}
	}
	if (PlayerRef->MainWidgetInstance)
	{
		PlayerRef->MainWidgetInstance->StopAiming();
	}
	ChargePower = MinimalPower;
}
