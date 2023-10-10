// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MetaballSplashSeparateComponent.h"

#include "ActorFactories/ActorFactorySkeletalMesh.h"
#include "Actors/MetaballChild.h"
#include "Actors/MetaballPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
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
		return;
	}
	
	UE_LOG(LogMetaballSplashSeparateComponent, Error, TEXT("OwnerStaticMeshComponent is null! Try set StaticMeshComponent in Actor!"));
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
			10.0f);

		if (MetaballVelocity.Length() > MinimalSeparateVelocity)
		{
			auto MetalballTransform = GetOwner()->GetActorTransform();
			MetalballTransform.SetScale3D(MetalballTransform.GetScale3D() / 2);
			GetWorld()->SpawnActor<AMetaballChild>(ChildMetaballClass, MetalballTransform);
		}
	}
	else
	{
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

