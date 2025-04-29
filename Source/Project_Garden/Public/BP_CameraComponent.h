#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BP_CameraComponent.generated.h"

class AMyProject8Character; // Déclaration avancée

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_GARDEN_API UBP_CameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBP_CameraComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TObjectPtr<AMyProject8Character> CharaRef;
};
