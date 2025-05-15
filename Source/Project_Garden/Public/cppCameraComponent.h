// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "cppCameraComponent.generated.h"

class AMyProject8Character;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GARDEN_API UcppCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UcppCameraComponent();

	UPROPERTY(EditAnywhere, Blueprintable)
	float InterpSpeedlag;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TObjectPtr<AMyProject8Character> CharaRef;

	UFUNCTION()
	void ResetPostition();

	UPROPERTY()
	FVector Postition000 = FVector(0, 0, 0);
};
