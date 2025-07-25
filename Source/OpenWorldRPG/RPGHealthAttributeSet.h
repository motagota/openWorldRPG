// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGAttributeSet.h"
#include "AbilitySystemComponent.h" // Added for UAbilitySystemComponent definition
#include "RPGHealthAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API URPGHealthAttributeSet : public URPGAttributeSet
{
	GENERATED_BODY()
public:
	URPGHealthAttributeSet();
	

	

	mutable FRPGAttributeEvent OnHealthChanged;
	mutable FRPGAttributeEvent OnMaxHealthChanged;
	mutable FRPGAttributeEvent OnOutOfHealth;

	// Add this missing function declaration
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_PoisonMeter(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_FrostMeter(const FGameplayAttributeData& OldValue);
	

public:  // Moved to public section
	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health= 100.0f;
	ATTRIBUTE_ACCESSORS(URPGHealthAttributeSet, Health);
		
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth = 100.0f;
	ATTRIBUTE_ACCESSORS(URPGHealthAttributeSet, MaxHealth);

	//STATUS effects
	
	UPROPERTY(BlueprintReadOnly, Category="Status", ReplicatedUsing=OnRep_PoisonMeter)
	FGameplayAttributeData PoisonMeter=1.0f;;
	ATTRIBUTE_ACCESSORS(URPGHealthAttributeSet, PoisonMeter);

	UPROPERTY(BlueprintReadOnly, Category="Status", ReplicatedUsing=OnRep_FrostMeter)
	FGameplayAttributeData FrostMeter=1.0f;;
	ATTRIBUTE_ACCESSORS(URPGHealthAttributeSet, FrostMeter);
	
private:
	bool bOutOfHealth;
};
