// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MetaballChild.h"

#include "Actors/MetaballPawn.h"
#include "Components/MetaballHealthComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMetaballChild::AMetaballChild()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(MeshComponent);

	HealthComponent = CreateDefaultSubobject<UMetaballHealthComponent>(TEXT("HealthComponent"));
}

void AMetaballChild::BeginPlay()
{
	Super::BeginPlay();

	if (const auto MetaballPawn = Cast<AMetaballPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		MetaballPawn->OnSwipe.AddDynamic(this, &AMetaballChild::HandleSwipeMovement);
		MainMetaball = MetaballPawn;
	}
}

FVector AMetaballChild::CalculateAttachForceVector()
{
	return CalculateDistanceToMainMetaball() * AttachmentForce;
}

FVector AMetaballChild::CalculateDistanceToMainMetaball() const
{
	const auto MainMetaballLocation = MainMetaball->GetActorLocation();
	const auto ChildMetaballLocation = GetActorLocation();
	return UKismetMathLibrary::GetDirectionUnitVector(ChildMetaballLocation, MainMetaballLocation);
}

void AMetaballChild::DecreaseSize()
{
	const auto NewScale = GetActorScale3D() * (1.0f - SizeDecreaseAmount);

	if (NewScale.X < MinimalSize || NewScale.Y < MinimalSize || NewScale.Z < MinimalSize)
	{
		UE_LOG(LogTemp, Warning, TEXT("Size is less than the minimal size. Stopping size decrease."));
		GetWorld()->GetTimerManager().ClearTimer(DecreaseSizeTimer);
		Destroy();
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Size decreased successfully."));
	SetActorScale3D(GetActorScale3D() * (1.0f - SizeDecreaseAmount));
}

// Called every frame
void AMetaballChild::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MeshComponent)
	{
		MeshComponent->AddForce(CalculateAttachForceVector() * InitialForce);
		const auto DistanceToMainMetaball = MainMetaball->GetActorLocation() - GetActorLocation();
		
		if (DistanceToMainMetaball.Length() > DieDistance && !bIsFarAwayFromMainMetaball)
		{
			bIsFarAwayFromMainMetaball = true;
			UE_LOG(LogTemp, Warning, TEXT("Distance to main metaball is greater than DieDistance"));
			GetWorld()->GetTimerManager().SetTimer(
				DecreaseSizeTimer,
				this,
				&AMetaballChild::DecreaseSize,
				SizeDecreaseTime,
				true);
		}

		if (DistanceToMainMetaball.Length() < DieDistance && bIsFarAwayFromMainMetaball)
		{
			bIsFarAwayFromMainMetaball = false;
			GetWorld()->GetTimerManager().ClearTimer(DecreaseSizeTimer);
		}
	}
}

void AMetaballChild::HandleSwipeMovement(const FVector& SwipeDirection)
{
	MeshComponent->AddForce(SwipeDirection);
}
