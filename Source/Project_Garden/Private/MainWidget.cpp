#include "MainWidget.h"
#include "TimerManager.h"
#include "Components/CanvasPanel.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"


void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (CrosshairImage)
	{
		CrosshairImage->SetVisibility(ESlateVisibility::Hidden);
	}
	
}



void UMainWidget::StartAiming()
{
	UE_LOG(LogTemp, Warning, TEXT("StartAiming called"));

	// Exemple : lancer un timer pour GrowCrosshair
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(AimTimerHandle, this, &UMainWidget::GrowCrosshair, 0.01f, true);
		if (CrosshairImage)
		{
			CrosshairImage->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UMainWidget::StopAiming()
{
	UE_LOG(LogTemp, Warning, TEXT("StopAiming called"));

	// Arrêter le timer si actif
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(AimTimerHandle);
		if (CrosshairImage)
		{
			CrosshairImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UMainWidget::GrowCrosshair()
{
	UE_LOG(LogTemp, Warning, TEXT("GrowCrosshair tick"));
	
}
