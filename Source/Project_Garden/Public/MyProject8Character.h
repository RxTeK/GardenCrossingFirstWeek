// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "GrapPoint.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CableComponent.h"
#include "MyProject8Character.generated.h"


class USwimComponent;
class USlingshotComponent;
class UClimbingComponent;
class USpringArmComponent;
class USlowFallComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class ASpline;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMyProject8Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Interaction Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SlingshotAction;

public:
	AMyProject8Character();
	std::vector <AGrapPoint*> GrapPoints;
	bool OnSpline = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Saut")
	USlowFallComponent* SlowFallComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Glide")
	bool bIsGliding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Glide")
	bool bIsStartingGlide;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Glide")
	bool bIsEndingGlide;

 UFUNCTION()
	void ResetGlide();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASpline> SplineClass;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ASpline* Rope;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Interaction();

	UFUNCTION()
	float height(AGrapPoint* Point);

	



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	USphereComponent* Sphere;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USwimComponent* SwimComponentRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USlingshotComponent* SlingshotComponentRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClimbingComponent* ClimbingComponentRef;

	UPROPERTY(EditAnywhere)
	UCableComponent* CableComponentRef;
	
	UPROPERTY()
	AGrapPoint* BestGrapPoint;
	
	UPROPERTY()
	bool bAttached = false;

	UPROPERTY()
	FVector2D MovementVector;
	
private:

	UPROPERTY()
	bool bCanJump;
	
	UFUNCTION()
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	
	UPROPERTY()
	float JumpImpulse = 700.0f;

	UPROPERTY()
	FVector LaunchVelocity = FVector(0, 0, 700);

	UPROPERTY()
	float CanJumpDuration = 0.2f;

	FTimerHandle JumpResetTimerHandle;

	UFUNCTION()
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void newJump();

	UFUNCTION()
	void newStopJumping();
	
	UFUNCTION()
	void Plane();

	UPROPERTY()
	bool bHit;

	UFUNCTION()
	void OnMovementFinish();
};

