// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "DataFragmentsAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class OPENWORLDRPG_API UDataFragmentsAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UDataFragmentsAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;	
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "DataFragments", ReplicatedUsing = OnRep_Datafragments)
	FGameplayAttributeData DataFragments;
	ATTRIBUTE_ACCESSORS(UDataFragmentsAttributeSet, DataFragments)

protected:
	UFUNCTION()
	virtual void OnRep_Datafragments(const FGameplayAttributeData& OldDataFragments);
	
	
};
