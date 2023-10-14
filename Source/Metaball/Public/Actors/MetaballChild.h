// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MetaballChild.generated.h"

class AMetaballPawn;

UCLASS()
class METABALL_API AMetaballChild : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMetaballChild();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleSwipeMovement(const FVector& SwipeDirection);

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USphereComponent* SphereComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float AttachmentForce;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector InitialForce;

	UPROPERTY(EditDefaultsOnly)
	float MinimalSize;

	UPROPERTY(EditDefaultsOnly)
	float DieDistance;

	UPROPERTY(EditDefaultsOnly, meta = (UIMin = "0.0", UIMax = "1.0"))
	float SizeDecreaseAmount;

	UPROPERTY(EditDefaultsOnly)
	float SizeDecreaseTime;

	UPROPERTY(EditDefaultsOnly)
	class UMetaballHealthComponent *HealthComponent;

private:
	UPROPERTY()
	AMetaballPawn* MainMetaball;

	FVector CalculateAttachForceVector();

	FVector CalculateDistanceToMainMetaball() const;
	
	FTimerHandle DecreaseSizeTimer;

	void DecreaseSize();

	bool bIsFarAwayFromMainMetaball;
};
