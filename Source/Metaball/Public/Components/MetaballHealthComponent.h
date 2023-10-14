// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MetaballHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnHealthChange, float, float, const UDamageType*)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METABALL_API UMetaballHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMetaballHealthComponent();
	
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth();

	UFUNCTION()
	bool IsDead() const;

	UFUNCTION()
	void OnTakeAnyDamageEvent(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser );

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent();

	FOnDeath OnDeathEvent;
	FOnHealthChange OnHealthChangeEvent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealHealth")
	bool bAutoHeal = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealHealth")
	float HealTimeUpdate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealHealth")
	float StartHealDelay = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealHealth")
	float HealModifier = 1.0f;

private:
	float Health = 100.0f;
	FTimerHandle HealTimerHandle;

	void HealUpdate();
};
