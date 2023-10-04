// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MetaballPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwipe, FVector, SwipeDirection);

UENUM()
enum class EInputState
{
	
	Pressed UMETA(DisplayName = "Pressed"),
	Released UMETA(DisplayName = "Released"),
};

USTRUCT()
struct FCurrentInputAction
{
	GENERATED_BODY()

public:
	FCurrentInputAction() : InputState(EInputState::Released), SwipeDirection(FVector::ZeroVector) {};

	EInputState InputState;
	FVector SwipeDirection;
};

UCLASS()
class METABALL_API AMetaballPawn : public APawn
{
	GENERATED_BODY()

public:
	AMetaballPawn();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float MinimalSwipeLength;

	void SwipeDown(const struct FInputActionValue& Value);

	void SwipeUp(const struct FInputActionValue& Value);

	UPROPERTY(BlueprintAssignable, Category = "Inventory System | Events")
	FOnSwipe OnSwipe;

	FCurrentInputAction SwipeInputAction;
};
