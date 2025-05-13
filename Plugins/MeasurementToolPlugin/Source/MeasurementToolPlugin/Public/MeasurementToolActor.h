#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeasurementToolActor.generated.h"

UCLASS()
class MEASUREMENTTOOLPLUGIN_API AMeasurementToolActor : public AActor
{
    GENERATED_BODY()

public:
    AMeasurementToolActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;
};
