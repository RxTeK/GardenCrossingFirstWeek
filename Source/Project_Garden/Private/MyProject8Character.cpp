// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProject8Character.h"

#include <string>

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GrapPoint.h"
#include "InputActionValue.h"
#include "KismetTraceUtils.h"
#include "VectorTypes.h"
#include "Components/SphereComponent.h"
#include "DataWrappers/ChaosVDQueryDataWrappers.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMyProject8Character

AMyProject8Character::AMyProject8Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMyProject8Character::OnComponentOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AMyProject8Character::OnComponentEndOverlap);

	SlowFallComponent = CreateDefaultSubobject<USlowFallComponent>(TEXT("SlowFallComponent"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMyProject8Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AMyProject8Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GrapPoints.size() > 0)
	{
		BestGrapPoint = GrapPoints[0];
		for (AGrapPoint* GrapPoint : GrapPoints)
		{
			GrapPoint->CanGrap(false);
			if (height(BestGrapPoint) > height(GrapPoint))
			{
				BestGrapPoint = GrapPoint;
			}
		}
		BestGrapPoint->CanGrap(true);
	}
	else if (BestGrapPoint != nullptr)
	{
		BestGrapPoint = nullptr;
	}
}

void AMyProject8Character::OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGrapPoint* NewPoint = Cast<AGrapPoint>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Grap Points!"));
		GrapPoints.push_back(NewPoint);
	}
}

void AMyProject8Character::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AGrapPoint* NewPoint = Cast<AGrapPoint>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Grap Points None!"));
		NewPoint->CanGrap(false);
		if (std::ranges::find(GrapPoints, NewPoint) != GrapPoints.end())
		{
			GrapPoints.erase(std::ranges::find(GrapPoints, NewPoint));
		}
		
	}
}

void AMyProject8Character::newJump()
{
	UE_LOG(LogTemp, Error, TEXT("Jump"));
	bPressedJump = true;
	JumpKeyHoldTime = 0.0f;
	if (bCanJump)
	{
		LaunchCharacter(LaunchVelocity, false, false);
	}
	else
	{
		if (USlowFallComponent* FallComp = Cast<USlowFallComponent>(SlowFallComponent))
		{
			FallComp->SlowFallOn();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("SlowFallComponent non initialisé !"));
		}
	}
}

void AMyProject8Character::newStopJumping()
{
	
}

void AMyProject8Character::HandleJumpInput()
{
	if (bCanJump)
	{
		bCanJump = false;
		GetWorldTimerManager().ClearTimer(JumpResetTimerHandle);

		// Saut vers le haut
		LaunchCharacter(FVector(0.f, 0.f, JumpImpulse), false, false);

		// Appel du slow fall
		if (SlowFallComponent)
		{
			UFunction* SlowFallFunc = SlowFallComponent->FindFunction(FName("SlowFallOn"));
			if (SlowFallFunc)
			{
				SlowFallComponent->ProcessEvent(SlowFallFunc, nullptr);
			}
		}
	}
}

void AMyProject8Character::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		bCanJump = true;

		GetWorldTimerManager().SetTimer(
			JumpResetTimerHandle,
			this,
			&AMyProject8Character::DisableJump,
			CanJumpDuration,
			false
		);
	}
}

void AMyProject8Character::DisableJump()
{
	bCanJump = false;
}


//////////////////////////////////////////////////////////////////////////
// Input

void AMyProject8Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyProject8Character::newJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyProject8Character::newStopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyProject8Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyProject8Character::Look);

		//Interact
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &AMyProject8Character::Interaction);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMyProject8Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyProject8Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMyProject8Character::Interaction()
{
	
}

float AMyProject8Character::height(AGrapPoint* Point)
{
	float Height = 0.0f;
	//Height = Point->GetDistanceTo(this);
	Height += UE::Geometry::Dot(Point->Arrow->GetForwardVector(), this->GetFollowCamera()->GetForwardVector());
	GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Yellow,FString::Printf(TEXT("Height = %f"), Height));
	return Height;
}
