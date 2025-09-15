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
#include "SlowFallComponent.h"
#include "Spline.h"
#include "MainWidget.h"
#include "SwimComponent.h"
#include "ClimbingComponent.h"
#include "FrameTypes.h"
#include "SlingshotComponent.h"
#include "DataWrappers/ChaosVDQueryDataWrappers.h"
#include "DSP/SpectrumAnalyzer.h"
#include "Kismet/KismetMathLibrary.h"

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
	
	SwimComponentRef = CreateDefaultSubobject<USwimComponent>(TEXT("SwimComponent"));
	
	SlingshotComponentRef = CreateDefaultSubobject<USlingshotComponent>(TEXT("SlingshotComponent"));

	ClimbingComponentRef = CreateDefaultSubobject<UClimbingComponent>(TEXT("ClimbingComponent"));

	CableComponentRef = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent"));
	CableComponentRef->SetupAttachment(RootComponent);

	

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMyProject8Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	if (MainWidget && GetWorld())
	{
		MainWidgetInstance = CreateWidget<UMainWidget>(GetWorld(), MainWidget);
		if (MainWidgetInstance)
		{
			MainWidgetInstance->AddToViewport();
		}
	}
	
}

void AMyProject8Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bAttached)
	{
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
			if (height(BestGrapPoint) >= 100.0f)
			{
				bAttached = false;
				BestGrapPoint = nullptr;
			}
			else
			{
				BestGrapPoint->CanGrap(true);
			}
		}
		else if (BestGrapPoint != nullptr)
		{
			BestGrapPoint = nullptr;
		}
	}
	else if (std::ranges::find(GrapPoints, BestGrapPoint) == GrapPoints.end())
	{
		BestGrapPoint = nullptr;
		bAttached = false;
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
	bPressedJump = true;
	bCanJump = false;

	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.0f, 0.0f, 200.0f);
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f, 0, 2.0f);
	if (bAttach == true)
	{
		DetachPlayer();
	}
	if (bCanJump)
	{
		LaunchCharacter(LaunchVelocity, false, false);
	}

	if (ClimbingComponentRef != nullptr)
	{
		if (ClimbingComponentRef->Climb() && !IsJumpOnClimb && !ClimbingComponentRef->PastOnTop)
		{
			IsJumpOnClimb = true;
			FVector Climb = GetActorLocation() ;
			FVector Launch = GetVelocity().GetSafeNormal() * (LaunchLenght ) + Climb ;
			DrawDebugLine(GetWorld(), GetActorLocation(), Launch, FColor::Blue, false, 10.0f, 0, 1.0f);
			PositionPlayerForLerp = GetActorLocation();
			LerpAlpha = 0.f;
			SetActorRotation(FRotator(UKismetMathLibrary::MakeRotFromX(End)));
			GetWorldTimerManager().ClearTimer(ClimbTimerHandle);
			GetWorldTimerManager().SetTimer(ClimbTimerHandle, [this, Launch](){this->JumpWall(Launch);}, GetWorld()->DeltaTimeSeconds, true);
			
		}
	}
	
	
}

void AMyProject8Character::JumpWall(FVector End)
{
	LerpAlpha += GetWorld()->DeltaTimeSeconds;
	FVector NewLocation = FMath::Lerp(PositionPlayerForLerp, End, LerpAlpha);
	SetActorLocation(NewLocation);
	if (LerpAlpha >= 0.1f)
	{
		SetActorLocation(End);
		this->GetCharacterMovement()->GravityScale = Gravity;
		IsJumpOnClimb = false;
		ClimbingComponentRef->Climb();
		
		GetWorldTimerManager().ClearTimer(ClimbTimerHandle);
	}
}

void AMyProject8Character::newStopJumping()
{
	if (!bIsGliding)
	{
		return;
	}

	if (SlowFallComponent)
	{
		SlowFallComponent->GravityClassic();
	}

	bIsStartingGlide = false;
	bIsEndingGlide = true;
}

void AMyProject8Character::Plane()
{
	if (bHasGlided)
	{
		return;
	}
	if (GetCharacterMovement()->IsFalling() && !ClimbingComponentRef->Climb() && !SwimComponentRef->Grabbed && !bAttach)
	{
		if(!bHit)
		{
			if (USlowFallComponent* FallComp = Cast<USlowFallComponent>(SlowFallComponent))
			{
				FallComp->SlowFallOn();
				bIsStartingGlide = true;
				bIsGliding = true;
				bIsEndingGlide = false;
				bHasGlided = true;
			}
		}
	}
}

void AMyProject8Character::OnMovementFinish()
{
	MovementVector.X = 0.0f;
	MovementVector.Y = 0.0f;
}


void AMyProject8Character::ResetGlide()
{
	UE_LOG(LogTemp, Warning, TEXT("Reset"));
	bIsGliding = false;
	bIsStartingGlide = false;
	bIsEndingGlide = false;
}

void AMyProject8Character::OnMoveCompleted()
{
	this->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	EnableInput(this->GetLocalViewingPlayerController());
}


void AMyProject8Character::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (GetCharacterMovement()->MovementMode == MOVE_Walking)
	{
		bHasGlided = false;
	}
	
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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Ongoing, this, &AMyProject8Character::Plane);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &AMyProject8Character::newStopJumping);
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyProject8Character::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &AMyProject8Character::OnMovementFinish);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMyProject8Character::OnMovementFinish);
		

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyProject8Character::Look);

		//Interact
		if (SwimComponentRef)
		{
			EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, SwimComponentRef, &USwimComponent::GrabStart);
			EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Completed, SwimComponentRef, &USwimComponent::GrabEnd);
			EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Canceled, SwimComponentRef, &USwimComponent::GrabEnd);
		}

		if (SlingshotComponentRef)
		{
			EnhancedInputComponent->BindAction(SlingshotAction, ETriggerEvent::Triggered, SlingshotComponentRef, &USlingshotComponent::ShootStart);
			EnhancedInputComponent->BindAction(SlingshotAction, ETriggerEvent::Completed, SlingshotComponentRef, &USlingshotComponent::ShootEnd);
			EnhancedInputComponent->BindAction(SlingshotAction, ETriggerEvent::Canceled, SlingshotComponentRef, &USlingshotComponent::ShootEnd);
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMyProject8Character::DetachPlayer()
{
	bAttach = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	FVector DetachImpulse = GetActorForwardVector() * 300.0f + FVector(0,0,200.f);
	LaunchCharacter(DetachImpulse, true, true);
}

void AMyProject8Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && ClimbingComponentRef)
	{
		if (ClimbingComponentRef->Climb())
		{
			const FRotator Rotation = GetActorRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			
			FVector UpDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			if (ClimbingComponentRef->OnGround && FMath::Sign(MovementVector.Y) < 0.0f)
			{
				UpDirection =  FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			}

			if (!ClimbingComponentRef->CanClimbUpOrDown(MovementVector.Y) && FMath::Sign(MovementVector.Y) >= 0.0f && ClimbingComponentRef->Climb())
			{
				FVector NewLaunchForward = FVector(GetActorLocation() + (GetActorForwardVector() * 50.0f));
				FVector EndClimbUpDirection = FVector(NewLaunchForward.X,NewLaunchForward.Y,GetActorLocation().Z + 104.0f);
				FRotator RotatorPlayer = GetActorRotation();
				
				FLatentActionInfo LatentInfo;
				LatentInfo.CallbackTarget = this;
				LatentInfo.ExecutionFunction = FName("OnMoveCompleted");
				LatentInfo.Linkage = 0;
				LatentInfo.UUID = FMath::Rand();

				UKismetSystemLibrary::MoveComponentTo(this->GetCapsuleComponent(), EndClimbUpDirection, RotatorPlayer, true, true, 1.0f, true, EMoveComponentAction::Move, LatentInfo);
				DisableInput(this->GetLocalViewingPlayerController());
			}

			else if (ClimbingComponentRef->CanClimbUpOrDown(MovementVector.Y))
			{
				AddMovementInput(UpDirection, MovementVector.Y);
			}

			if (ClimbingComponentRef->CanClimbLeftOrRight(MovementVector.X))
			{
				AddMovementInput(RightDirection, MovementVector.X);
			}
		}
		else
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
		
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

	if (bAttached)
	{
		bAttached = false;
		this->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		this->LaunchCharacter(Rope->SkeletalMesh->GetBoneLinearVelocity(FName(TEXT("Bone_008"))),false,false);
		Rope->K2_DestroyActor();
	}
	
	else if (BestGrapPoint != nullptr)
	{
		UWorld* World = GetWorld();
		
		if (World != nullptr && SplineClass != nullptr && !bAttached)
		{
			bAttached = true;
			Rope = World->SpawnActor<ASpline>(SplineClass, BestGrapPoint->GetActorLocation(), FRotator(0, 0, 0));
			Rope->AttachToActor(BestGrapPoint, FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("Bone")));
			//Rope->SetActorLocation(BestGrapPoint->GetActorLocation(), false , nullptr, ETeleportType::TeleportPhysics);
			this->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			this->GetCharacterMovement()->StopMovementImmediately();
			this->AttachToActor(Rope, FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("Bone_008")));
			this->AddActorLocalRotation(FRotator(90.0f,0.0f,90.0f));
		}
	}
	
}

float AMyProject8Character::height(const AGrapPoint* Point)
{
	float Height = Point->GetDistanceTo(this) / 100.0f;
	if (UE::Geometry::Dot(Point->Arrow->GetForwardVector(), this->GetFollowCamera()->GetForwardVector()) < 0.0f)
	{
		Height += UE::Geometry::Dot(Point->Arrow->GetForwardVector(), this->GetFollowCamera()->GetForwardVector());
	}
	else
	{
		Height += 500.0f;
	}
	GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Yellow,FString::Printf(TEXT("Height = %f"), Height));
	return Height;
}
