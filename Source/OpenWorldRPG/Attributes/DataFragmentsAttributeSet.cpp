// Fill out your copyright notice in the Description page of Project Settings.


#include "DataFragmentsAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UDataFragmentsAttributeSet::UDataFragmentsAttributeSet()
{
	InitDataFragments(0.0f);
}

void UDataFragmentsAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDataFragmentsAttributeSet, DataFragments, COND_None, REPNOTIFY_Always);	
}


void UDataFragmentsAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	
}

void UDataFragmentsAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

}

void UDataFragmentsAttributeSet::OnRep_Datafragments(const FGameplayAttributeData& OldDataFragments)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDataFragmentsAttributeSet, DataFragments, OldDataFragments);
}

