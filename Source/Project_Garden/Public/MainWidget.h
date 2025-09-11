#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
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
	

	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite)
	UCanvasPanel* RootCanvas;

	// Image enfant
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite)
	UImage* CrosshairImage;

	
protected:
	virtual void NativeConstruct() override;


private:
	FTimerHandle AimTimerHandle;
};
