// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/Image.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "GrapPoint.generated.h"


class UGrapPointWidget;

UCLASS()
class PROJECT_GARDEN_API AGrapPoint : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Box;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* Widget;

	
	
public:	
	// Sets default values for this actor's properties
	AGrapPoint();
	
	void CanGrap(bool);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FSlateBrush GrabbedImage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FSlateBrush CantGrabImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UArrowComponent* Arrow;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	UGrapPointWidget* GrapPointWidget;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
