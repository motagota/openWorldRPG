// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectComponent.h"

#include "GameplayTagContainer.h"


UStatusEffectComponent::UStatusEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatusEffectComponent::OnGameplayCueTriggered(FName CueTag, float NormalizedValue)
{
	const FString CueString = CueTag.ToString();
	UE_LOG(LogTemp, Warning, TEXT("UStatusEffectComponent::OnGameplayCueTriggered - CueTag: %s"), *CueString);
	UE_LOG(LogTemp, Warning, TEXT("UStatusEffectComponent::OnGameplayCueTriggered - NormalizedValue: %f"), NormalizedValue);
	if (!StatusEffectContainerWidget) return;


	FString statusType;
	if ( CueString.Split(TEXT("GameplayCue.GC.STATUS."), nullptr, &statusType) )
	{
		FGameplayTag statusTag = FGameplayTag::RequestGameplayTag(*FString::Printf(TEXT("GE.STATUS.%s"),*statusType));
		StatusEffectContainerWidget->ShowOrUpdateStatus(statusTag,NormalizedValue);	
	}
	
}


void UStatusEffectComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("UStatusEffectComponent::BeginPlay"));

	if (StatusEffectContainerWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UStatusEffectComponent::BeginPlay StatusEffectContainerWidgetClass"));

		StatusEffectContainerWidget = CreateWidget<UStatusContainerWidget>(GetWorld(), StatusEffectContainerWidgetClass);

		if (StatusEffectContainerWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("UStatusEffectComponent::BeginPlay StatusEffectContainerWidget"));
			StatusEffectContainerWidget->AddToViewport();
			StatusEffectContainerWidget->SetVisibility(ESlateVisibility::Visible);

		}
	}
}

void UStatusEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
