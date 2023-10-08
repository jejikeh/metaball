// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MetaballPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSwipe, FVector, SwipeDirection, float, SwipeVelocity);

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
	FCurrentInputAction() : InputState(EInputState::Released), SwipeDirection(FVector::ZeroVector),
	                        SwipeInputStartTime(0.0f), SwipeInputEndTime(0.0f)
	{
	};

	EInputState InputState;
	FVector SwipeDirection;
	float SwipeInputStartTime;
	float SwipeInputEndTime;

	FORCEINLINE float GetSwipeTime() const
	{
		return SwipeInputEndTime - SwipeInputStartTime;
	}

	FORCEINLINE float CalculateSwipeVelocityModifier() const
	{
		// TODO: Add project settings
		// return FMath::Eval(SwipeDirection.Length() / GetSwipeTime());
		return (SwipeDirection.Length() / GetSwipeTime()) * 100.0f;
	}
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

	UPROPERTY(BlueprintReadWrite, Category = "Components")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float MinimalSwipeLength;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float SwipeInteractionTime;

	void SwipeDown(const struct FInputActionValue& Value);

	void SwipeUp(const FInputActionValue& Value);

	void TouchFinger(const FInputActionValue& Value);
	
	UPROPERTY(BlueprintAssignable, Category = "Inventory System | Events")
	FOnSwipe OnSwipe;

	FCurrentInputAction SwipeInputAction;
};
