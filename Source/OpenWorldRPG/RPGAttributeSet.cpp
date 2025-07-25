// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAttributeSet.h"


URPGAttributeSet::URPGAttributeSet()
{
	
}

UWorld* URPGAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);
	return Outer->GetWorld();
}



void URPGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    // Base implementation does nothing
}

