// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyRiddle.generated.h"

class AMyGateForRiddle;

UCLASS()
class PROJECT_GARDEN_API AMyRiddle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyRiddle();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	UPROPERTY()
	AMyGateForRiddle* MyGateForRiddle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Touched();

	UFUNCTION()
	void AddGate(AMyGateForRiddle* GateForRiddle);
	
};


