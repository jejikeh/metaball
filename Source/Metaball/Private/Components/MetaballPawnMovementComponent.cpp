// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MetaballPawnMovementComponent.h"

#include "MathUtil.h"
#include "Actors/MetaballPawn.h"
#include "Kismet/GameplayStatics.h"

void UMetaballPawnMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const auto Pawn = Cast<AMetaballPawn>(GetOwner()))
	{
		MetaballPawn = Pawn;
	}
}

bool UMetaballPawnMovementComponent::IsFalling() const
{
	return MetaballPawn->GetVelocity().Z < FallingVelocityThreshold;
}
