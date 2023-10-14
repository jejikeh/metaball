// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MetaballHealthComponent.h"

UMetaballHealthComponent::UMetaballHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMetaballHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
    
	if (const auto Owner = GetOwner())
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UMetaballHealthComponent::OnTakeAnyDamageEvent);
	}
}

float UMetaballHealthComponent::GetCurrentHealth()
{
	return Health;
}

bool UMetaballHealthComponent::IsDead() const
{
	return FMath::IsNearlyZero(Health);
}

void UMetaballHealthComponent::OnTakeAnyDamageEvent(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

	OnHealthChangeEvent.Broadcast(Health, -Damage, DamageType);
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    
	if (IsDead())
	{
		OnDeathEvent.Broadcast();
	}
	else if (bAutoHeal) 
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UMetaballHealthComponent::HealUpdate, HealTimeUpdate, true, StartHealDelay);
	}
}

float UMetaballHealthComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

void UMetaballHealthComponent::HealUpdate()
{
	Health = FMath::Clamp(Health + HealModifier, 0.0f, MaxHealth);

	OnHealthChangeEvent.Broadcast(Health, HealModifier, nullptr);

	if (FMath::IsNearlyEqual(Health, MaxHealth))
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}
