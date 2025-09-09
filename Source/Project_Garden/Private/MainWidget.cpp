#include "MainWidget.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

void UMainWidget::StartAiming()
{
	UE_LOG(LogTemp, Warning, TEXT("StartAiming called"));

	// Exemple : lancer un timer pour GrowCrosshair
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(AimTimerHandle, this, &UMainWidget::GrowCrosshair, 0.01f, true);
	}
}

void UMainWidget::StopAiming()
{
	UE_LOG(LogTemp, Warning, TEXT("StopAiming called"));

	// Arrêter le timer si actif
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(AimTimerHandle);
	}
}

void UMainWidget::GrowCrosshair()
{
	UE_LOG(LogTemp, Warning, TEXT("GrowCrosshair tick"));

	// Ici tu peux ajouter ton code pour agrandir la croix de visée
	// Par exemple modifier un UImage ou une variable float
}
