// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MetaballSplashSeparateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChildChanged, int32, ChildCount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMetaballChildBelowThreshold);

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

	UFUNCTION(BlueprintCallable)
	int32 GetSeparateCount() const;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentChildCount() const;

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

	UPROPERTY(EditDefaultsOnly)
	bool bIsCapableOfSeparating;

	UPROPERTY(EditDefaultsOnly)
	int32 MaximumSeparateCount;

	UPROPERTY(EditDefaultsOnly)
	int32 InitialChildCount;

	UPROPERTY(EditDefaultsOnly)
	bool bIsInMainMetaball;

	UFUNCTION()
	void HandleChildDeath(AActor* DestroyedActor);
	
	UPROPERTY(BlueprintAssignable)
	FOnChildChanged OnChildChanged;

	UPROPERTY(BlueprintAssignable)
	FOnMetaballChildBelowThreshold OnMetaballChildBelowThreshold;

private:
	bool bIsInSplash;

	bool bIsInitializeChild = false;

	UPROPERTY()
	UStaticMeshComponent* OwnerStaticMeshComponent;

	void HealSeparateCount();
	
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	bool IsVelocityMoreThanMinimalDesired(const FVector& Velocity) const;

	UMaterialInterface* GetRandomSplashMaterial() const;

	int32 SeparateCount;

	FTimerHandle HealSeparateCountTimer;

	int32 ChildCount;

	float MinimalChildThreshold;
};
