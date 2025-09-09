#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

UCLASS()
class PROJECT_GARDEN_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void StartAiming();

	UFUNCTION(BlueprintCallable)
	void StopAiming();

	void GrowCrosshair();

private:
	FTimerHandle AimTimerHandle;
};
