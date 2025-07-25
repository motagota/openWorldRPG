// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoisonStatusWidget.h"
#include "Components/ActorComponent.h"
#include "PoisonStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPENWORLDRPG_API UPoisonStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPoisonStatusComponent();
	void OnGameplayCueTriggered(FName CueTag);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PoisonStatusWidgetClass;

	// Forward declare the widget class if it exists in another file
	UPROPERTY()
	UPoisonStatusWidget* PoisonStatusWidget;

	float PoisonLevel;
	float PoisonMax = 100.0f;


		
};
