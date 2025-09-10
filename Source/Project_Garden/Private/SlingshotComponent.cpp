// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectilActor.h"
#include "SlingshotComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


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
	TickBase = 0.01f;
	// ...
	
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
		ChargePower += TickBase * 5.f;
		ChargePower = FMath::Clamp(ChargePower,MinimalPower,MaximalPower);
		FString Msg = FString::Printf(TEXT("La valeur est : %f"), ChargePower);
		FRotator CameraRotation = PlayerRef->GetFollowCamera()->GetComponentRotation();
		PlayerRef->SetActorRotation(FRotator(PlayerRef->GetActorRotation().Pitch, CameraRotation.Yaw, PlayerRef->GetActorRotation().Roll));
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, Msg);
		GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Blue,TEXT("ChargePower"));
	}
	
	
}

void USlingshotComponent::ShootEnd()
{
	if (ProjectileClass && PlayerRef)
	{
		GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Blue,"Shoot");
		FVector MuzzleLocation = PlayerRef->GetActorLocation() + PlayerRef->GetFollowCamera()->GetForwardVector() * 100.f;
		DrawDebugLine(GetWorld(), PlayerRef->GetActorLocation(), MuzzleLocation, FColor::Red, false, 10.0f, 0, 1.0f);
		FRotator MuzzleRotation = PlayerRef->GetFollowCamera()->GetComponentRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.Owner = PlayerRef;

		AProjectilActor* Projectile = GetWorld()->SpawnActor<AProjectilActor>(
			ProjectileClass,
			MuzzleLocation,
			MuzzleRotation,
			SpawnParams
		);
		
		if (Projectile != nullptr && Projectile->ProjectileMovement )
		{
			Projectile->MoveProjectile(ChargePower*20.0f,MuzzleRotation);
		}
	}
	ChargePower = 100.0f;
}
