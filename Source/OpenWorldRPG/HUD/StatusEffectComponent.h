// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusContainerWidget.h"
#include "Components/ActorComponent.h"
#include "StatusEffectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPENWORLDRPG_API UStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatusEffectComponent();
	void OnGameplayCueTriggered(FName CueTag, float NormalizedValue);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> StatusEffectContainerWidgetClass;

	UPROPERTY()
	UStatusContainerWidget* StatusEffectContainerWidget;	

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
