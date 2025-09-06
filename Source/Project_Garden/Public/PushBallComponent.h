#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PushBallComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GARDEN_API UPushBallComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPushBallComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Push")
	float TraceDistance = 60.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Push")
	float PushForce = 200000.0f;
};
