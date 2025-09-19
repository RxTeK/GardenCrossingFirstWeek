// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SlowFallComponent.generated.h"

class AMyProject8Character;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GARDEN_API USlowFallComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USlowFallComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void SlowFallOn();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SlowFall")
	float GravityScaleClassic;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SlowFall")
	float AirControlClassic;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SlowFall")
	float GravityScaleGlide;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SlowFall")
	float AirControlGlide;

	UFUNCTION()
	void GravityClassic();

	UPROPERTY()
	bool StopPlane;

	UPROPERTY()
	bool Plane = true;

	UPROPERTY(EditDefaultsOnly, Category = "Glider")
	TSubclassOf<AActor> GliderBPClass;

	UPROPERTY()
	AActor* GliderInstance;

	UPROPERTY(EditAnywhere, Category = "Glider")
	float MaxGlideTime = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = "Glider")
	float GlideDeteriorationRate = 1.0f;

	UPROPERTY()
<<<<<<< Updated upstream
<<<<<<< Updated upstream
	bool AlreadyPlane;
=======
	float NewGravity;

>>>>>>> Stashed changes
=======
	float NewGravity;

>>>>>>> Stashed changes
	
private:
	UPROPERTY()
	TObjectPtr<AMyProject8Character> CharaRef;

	

	UPROPERTY()
	float GlideTimer = 0.0f;

	UFUNCTION()
	void SpawnAndAttachGlider();

	UFUNCTION()
	void DestroyGlider();

	UPROPERTY()
	float GravityTemp;
};
