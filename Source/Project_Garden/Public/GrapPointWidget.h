// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "GrapPointWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GARDEN_API UGrapPointWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
	UPROPERTY(meta = (BindWidget), EditAnywhere)
	UImage* GrapPointImage;
};
