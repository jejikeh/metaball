// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MetaballPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/MetabalPlayerController.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMetaballPawn::AMetaballPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);

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

void AMetaballPawn::SwipeDown(const FInputActionValue& Value)
{
	SwipeInputAction.InputState = EInputState::Pressed;
	SwipeInputAction.SwipeDirection = Value.Get<FVector>();
	SwipeInputAction.SwipeInputStartTime = GetWorld()->GetTimeSeconds();
	UE_LOG(LogTemp, Warning, TEXT("SwipeDown: %f"), SwipeInputAction.SwipeInputStartTime);
}

void AMetaballPawn::SwipeUp(const FInputActionValue& Value)
{
	SwipeInputAction.InputState = EInputState::Released;
	SwipeInputAction.SwipeInputEndTime = GetWorld()->GetTimeSeconds();

	UE_LOG(LogTemp, Warning, TEXT("SwipeUp Time: %f"), SwipeInputAction.GetSwipeTime());
	UE_LOG(LogTemp, Warning, TEXT("SwipeUp: %s"), *SwipeInputAction.SwipeDirection.ToString());
	
	if (SwipeInputAction.SwipeDirection.Length() > MinimalSwipeLength)
	{
		OnSwipe.Broadcast(SwipeInputAction.SwipeDirection, SwipeInputAction.CalculateSwipeVelocityModifier());
	}
}

void AMetaballPawn::TouchFinger(const FInputActionValue& Value)
{
	SwipeInputAction.SwipeDirection += Value.Get<FVector>();
}

void AMetaballPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMetaballPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	const auto MetalballPlayerController = Cast<AMetabalPlayerController>(Controller);

	MetalballPlayerController->bShowMouseCursor = true;
	MetalballPlayerController->bEnableClickEvents = true;
	MetalballPlayerController->bEnableTouchEvents = true;
	
	EnhancedInputComponent->BindAction(MetalballPlayerController->SwipeActionDown, ETriggerEvent::Started, this, &AMetaballPawn::SwipeDown);
	EnhancedInputComponent->BindAction(MetalballPlayerController->SwipeActionUp, ETriggerEvent::Completed, this, &AMetaballPawn::SwipeUp);
	EnhancedInputComponent->BindAction(MetalballPlayerController->TouchFinger, ETriggerEvent::Triggered, this, &AMetaballPawn::TouchFinger);
	
	if (const auto LocalPlayer = MetalballPlayerController->GetLocalPlayer())
	{
		const auto Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(MetalballPlayerController->InputMappingContext, 0);
	}
}