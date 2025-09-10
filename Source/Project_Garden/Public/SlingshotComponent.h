// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject8Character.h"
#include "Components/ActorComponent.h"
#include "SlingshotComponent.generated.h"

class UProjectileActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GARDEN_API USlingshotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USlingshotComponent();

	UFUNCTION()
	void ShootStart();

	UFUNCTION()
	void ShootEnd();

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TSubclassOf<class AProjectilActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	float MinimalPower;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	float MaximalPower;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY()
	AMyProject8Character* PlayerRef;
	float ChargePower = MinimalPower;

	float TickBase;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
