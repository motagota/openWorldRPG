// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OpenWorldRPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API AOpenWorldRPGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Input Mode")
	void SetInputModeToUIOnly(UUserWidget* WidgetToFocus);

	UFUNCTION(BlueprintCallable, Category = "Input Mode")
	void SetInputModeToGameOnly();
	
};
