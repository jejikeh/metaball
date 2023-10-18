// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MetaballPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/MetaballChild.h"
#include "Actors/MetabalPlayerController.h"
#include "Components/MetaballPawnMovementComponent.h"
#include "Components/MetaballSplashSeparateComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMetaballPawn::AMetaballPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);

	MetaballPawnMovementComponent = CreateDefaultSubobject<UMetaballPawnMovementComponent>(TEXT("MetaballPawnMovementComponent"));
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(MeshComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(SphereComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void AMetaballPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AMetaballPawn::TapJumpDown(const FInputActionValue& Value)
{
	
}

void AMetaballPawn::TapJumpUp(const FInputActionValue& Value)
{
	SwipeInputAction.InputState = EInputState::Released;

	UE_LOG(LogTemp, Warning, TEXT("SwipeUp: %s"), *SwipeInputAction.SwipeDirection.ToString());
	
	if (SwipeInputAction.SwipeDirection.Length() > MinimalSwipeLength && MetaballPawnMovementComponent->IsFalling())
	{
		FVector SwipeDirectionVec = (FVector{SwipeInputAction.SwipeDirection.Y, SwipeInputAction.SwipeDirection.X, 0}) + JumpDefaultVelocity;
		ClampVector(&SwipeDirectionVec);

		OnSwipe.Broadcast(SwipeDirectionVec);

		HandleSwipeMovement(SwipeDirectionVec);
	}
}

void AMetaballPawn::TouchFingerX(const FInputActionValue& Value)
{
	SwipeInputAction.SwipeDirection.X = Value.Get<float>();
	SwipeInputAction.InputState = EInputState::Released;

	UE_LOG(LogTemp, Warning, TEXT("SwipeUp: %s"), *SwipeInputAction.SwipeDirection.ToString());
	
	if (MetaballPawnMovementComponent->IsFalling())
	{
		FVector SwipeDirectionVec = (FVector{0, SwipeInputAction.SwipeDirection.X * JumpDefaultVelocity.X, JumpDefaultVelocity.Z});
		ClampVector(&SwipeDirectionVec);
		OnSwipe.Broadcast(SwipeDirectionVec);
		HandleSwipeMovement(SwipeDirectionVec);
	}
}

void AMetaballPawn::TouchFingerY(const FInputActionValue& Value)
{
	SwipeInputAction.SwipeDirection.Y = Value.Get<float>();
	SwipeInputAction.InputState = EInputState::Released;

	UE_LOG(LogTemp, Warning, TEXT("SwipeUp: %s"), *SwipeInputAction.SwipeDirection.ToString());
	
	if (MetaballPawnMovementComponent->IsFalling())
	{
		FVector SwipeDirectionVec = (FVector{SwipeInputAction.SwipeDirection.Y * JumpDefaultVelocity.Y, 0, JumpDefaultVelocity.Y});
		ClampVector(&SwipeDirectionVec);
		OnSwipe.Broadcast(SwipeDirectionVec);
		HandleSwipeMovement(SwipeDirectionVec);
	}
}

void AMetaballPawn::TouchFingerUp(const FInputActionValue& Value)
{
	SwipeInputAction.InputState = EInputState::Pressed;
	SwipeInputAction.SwipeDirection = FVector::Zero();
}

void AMetaballPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("Swipe: %s"), *SwipeInputAction.SwipeDirection.ToString());
}

void AMetaballPawn::HandleSwipeMovement(const FVector& SwipeDirection)
{
	MeshComponent->AddForce(SwipeDirection);
}

void AMetaballPawn::ClampVector(FVector* Vector)
{
	Vector->X = FMath::Clamp(Vector->X, -MaxVelocity.X, MaxVelocity.X);
	Vector->Y = FMath::Clamp(Vector->Y, -MaxVelocity.Y, MaxVelocity.Y);
	Vector->Z = FMath::Clamp(Vector->Z, -MaxVelocity.Z, MaxVelocity.Z);
}

void AMetaballPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	const auto MetalballPlayerController = Cast<AMetabalPlayerController>(Controller);

	MetalballPlayerController->bShowMouseCursor = true;
	MetalballPlayerController->bEnableClickEvents = true;
	MetalballPlayerController->bEnableTouchEvents = true;
	
	EnhancedInputComponent->BindAction(MetalballPlayerController->TapJumpActionDown, ETriggerEvent::Started, this, &AMetaballPawn::TapJumpDown);
	EnhancedInputComponent->BindAction(MetalballPlayerController->TapJumpActionUp, ETriggerEvent::Started, this, &AMetaballPawn::TapJumpUp);
	EnhancedInputComponent->BindAction(MetalballPlayerController->TouchFingerX, ETriggerEvent::Triggered, this, &AMetaballPawn::TouchFingerX);
	EnhancedInputComponent->BindAction(MetalballPlayerController->TouchFingerY, ETriggerEvent::Triggered, this, &AMetaballPawn::TouchFingerY);
	EnhancedInputComponent->BindAction(MetalballPlayerController->TouchFingerUp, ETriggerEvent::Completed, this, &AMetaballPawn::TouchFingerUp);
	
	if (const auto LocalPlayer = MetalballPlayerController->GetLocalPlayer())
	{
		const auto Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(MetalballPlayerController->InputMappingContext, 0);
	}
}
