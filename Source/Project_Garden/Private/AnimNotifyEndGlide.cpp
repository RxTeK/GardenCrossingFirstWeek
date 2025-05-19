#include "AnimNotifyEndGlide.h"
#include "MyProject8Character.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyEndGlide::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	AMyProject8Character* Character = Cast<AMyProject8Character>(Owner);
	if (Character && Character->bIsGliding)
	{
		Character->ResetGlide();
	}
}
