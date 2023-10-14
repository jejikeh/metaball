// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MetaballPawnMovementComponent.generated.h"

class AMetaballPawn;
/**
 * 
 */
UCLASS()
class METABALL_API UMetaballPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual bool IsFalling() const override;

protected:
	UPROPERTY(EditDefaultsOnly)
	double FallingVelocityThreshold = 1.0;

private:
	UPROPERTY()
	AMetaballPawn* MetaballPawn;
};
