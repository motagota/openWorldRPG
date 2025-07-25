// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGHealthAttributeSet.h"
#include "Net/UnrealNetwork.h"

URPGHealthAttributeSet::URPGHealthAttributeSet()
: Health(100.0f)
	, MaxHealth(100.0f)
{
	bOutOfHealth = false;
}
void URPGHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URPGHealthAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGHealthAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void URPGHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	FString AttributeName = Attribute.GetName();

	// Log the attribute name and the new value
	UE_LOG(LogTemp, Warning, TEXT("Pre-attribute change: %s, New Value: %f"), *AttributeName, NewValue);

	
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		// Clamp the current health value to not exceed the new max health
		const float CurrentHealth = GetHealth();
		if (CurrentHealth > NewValue)
		{
			// If current health exceeds new max health, adjust it
			GetOwningAbilitySystemComponent()->SetNumericAttributeBase(GetHealthAttribute(), NewValue);
		}
	}
}
void URPGHealthAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGHealthAttributeSet, Health, OldValue);

	
	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	// These events on the client should not be changing attributes

	const float CurrentHealth = GetHealth();
	const float EstimatedMagnitude = CurrentHealth - OldValue.GetCurrentValue();
	
	OnHealthChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentHealth);

	if (!bOutOfHealth && CurrentHealth <= 0.0f)
	{
		OnOutOfHealth.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentHealth);
	}

	bOutOfHealth = (CurrentHealth <= 0.0f);
}

void URPGHealthAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGHealthAttributeSet, MaxHealth, OldValue);

	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	OnMaxHealthChanged.Broadcast(nullptr, nullptr, nullptr, GetMaxHealth() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetMaxHealth());
}

void URPGHealthAttributeSet::OnRep_PoisonMeter(const FGameplayAttributeData& OldValue){}

void URPGHealthAttributeSet::OnRep_FrostMeter(const FGameplayAttributeData& OldValue)
{
}
