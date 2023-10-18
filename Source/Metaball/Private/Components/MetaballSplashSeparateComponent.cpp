// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MetaballSplashSeparateComponent.h"

#include "MetaballDeveloperSettings.h"
#include "Actors/MetaballChild.h"
#include "Actors/MetaballPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogMetaballSplashSeparateComponent);

UMetaballSplashSeparateComponent::UMetaballSplashSeparateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UMetaballSplashSeparateComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerStaticMeshComponent = GetOwner()->GetComponentByClass<UStaticMeshComponent>();
	if (OwnerStaticMeshComponent)
	{
		OwnerStaticMeshComponent->OnComponentHit.AddDynamic(this, &UMetaballSplashSeparateComponent::OnComponentHit);

		const UMetaballDeveloperSettings* MetaballDeveloperSettings = GetDefault<UMetaballDeveloperSettings>();
		MinimalChildThreshold = MetaballDeveloperSettings->MinChildCount;
		return;
	}
	
	UE_LOG(LogMetaballSplashSeparateComponent, Error, TEXT("OwnerStaticMeshComponent is null! Try set StaticMeshComponent in Actor!"));
}

void UMetaballSplashSeparateComponent::HandleChildDeath(AActor* DestroyedActor)
{
	ChildCount--;
	OnChildChanged.Broadcast(ChildCount);

	if (ChildCount < MinimalChildThreshold)
	{
		OnMetaballChildBelowThreshold.Broadcast();
	}
}

void UMetaballSplashSeparateComponent::HealSeparateCount()
{
	if (SeparateCount >= MaximumSeparateCount)
	{
		UE_LOG(LogMetaballSplashSeparateComponent, Error, TEXT("Maximum Separate Count reached!"));
		GetWorld()->GetTimerManager().ClearTimer(HealSeparateCountTimer);
		return;
	}

	SeparateCount++;
	UE_LOG(LogMetaballSplashSeparateComponent, Log, TEXT("Separate Count: %d"), SeparateCount);
}

void UMetaballSplashSeparateComponent::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	const auto *MetaballPawn = Cast<AMetaballPawn>(OtherActor);
	const auto *MetaballChild = Cast<AMetaballChild>(OtherActor);
	if (MetaballPawn || MetaballChild)
	{
		return;
	}

	if (const auto MetaballVelocity = GetOwner()->GetVelocity(); IsVelocityMoreThanMinimalDesired(MetaballVelocity) && !bIsInSplash)
	{
		bIsInSplash = true;
		UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			GetRandomSplashMaterial(),
			InitialSplashScale,
			Hit.Location,
			UKismetMathLibrary::MakeRotFromX(Hit.Normal),
			3.0f);
		
		if (MetaballVelocity.Length() > MinimalSeparateVelocity && bIsCapableOfSeparating && SeparateCount > 0 || (bIsInMainMetaball && !bIsInitializeChild))
		{
			auto MetalballTransform = GetOwner()->GetActorTransform();
			MetalballTransform.SetScale3D(MetalballTransform.GetScale3D());

			auto ChildSpawn = FMath::RandRange(0, MaximumSeparateCount - SeparateCount);
			if (!bIsInitializeChild && bIsInMainMetaball)
			{
				const UMetaballDeveloperSettings* MetaballDeveloperSettings = GetDefault<UMetaballDeveloperSettings>();
				ChildSpawn = MetaballDeveloperSettings->MinChildCount;
				bIsInitializeChild = true;
			}
			
			for (int i = 0; i < ChildSpawn; i++)
			{
				const auto Child = GetWorld()->SpawnActor<AMetaballChild>(ChildMetaballClass, MetalballTransform);
				Child->OnDestroyed.AddDynamic(this, &UMetaballSplashSeparateComponent::HandleChildDeath);
				
				SeparateCount = FMath::Clamp(SeparateCount - 1, 0, MaximumSeparateCount);
				ChildCount++;

				OnChildChanged.Broadcast(ChildCount);
			}
		}
	}
	else
	{
		if (bIsInSplash && SeparateCount < MaximumSeparateCount)
		{
			GetWorld()->GetTimerManager().SetTimer(HealSeparateCountTimer, this, &UMetaballSplashSeparateComponent::HealSeparateCount, 1.0f, true);
		}
		
		bIsInSplash = false;
	}
}

bool UMetaballSplashSeparateComponent::IsVelocityMoreThanMinimalDesired(const FVector& Velocity) const
{
	return FMath::Abs(Velocity.X) > MinimalSplashVelocity.X || FMath::Abs(Velocity.Y) > MinimalSplashVelocity.Y || FMath::Abs(Velocity.Z) > MinimalSplashVelocity.Z;
}

UMaterialInterface* UMetaballSplashSeparateComponent::GetRandomSplashMaterial() const
{
	return SplashMaterials[FMath::RandRange(0, SplashMaterials.Num() - 1)];
}

void UMetaballSplashSeparateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int32 UMetaballSplashSeparateComponent::GetSeparateCount() const
{
	return SeparateCount;
}

int32 UMetaballSplashSeparateComponent::GetCurrentChildCount() const
{
	return ChildCount;
}

