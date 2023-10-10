// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MetaballSplashSeparateComponent.generated.h"

class AMetaballChild;
DECLARE_LOG_CATEGORY_EXTERN(LogMetaballSplashSeparateComponent, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METABALL_API UMetaballSplashSeparateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMetaballSplashSeparateComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TArray<UMaterialInterface*> SplashMaterials;

	UPROPERTY(EditDefaultsOnly)
	FVector MinimalSplashVelocity;

	UPROPERTY(EditDefaultsOnly)
	float MinimalSeparateVelocity;

	UPROPERTY(EditDefaultsOnly)
	FVector InitialSplashScale;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMetaballChild> ChildMetaballClass;

private:
	bool bIsInSplash;

	UPROPERTY()
	UStaticMeshComponent* OwnerStaticMeshComponent;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	bool IsVelocityMoreThanMinimalDesired(const FVector& Velocity) const;

	UMaterialInterface* GetRandomSplashMaterial() const;
};
