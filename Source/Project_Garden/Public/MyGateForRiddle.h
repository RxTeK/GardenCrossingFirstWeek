// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "GameFramework/Actor.h"
#include "MyGateForRiddle.generated.h"

class AMyRiddle;

UCLASS()
class PROJECT_GARDEN_API AMyGateForRiddle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyGateForRiddle();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<AMyRiddle*> Riddles;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void RemoveIsTouched(AMyRiddle* Riddle);
};


