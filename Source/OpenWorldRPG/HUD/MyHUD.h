// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UPlayerStatBars> PlayerStBarsWidget;

	UPROPERTY(BlueprintReadWrite, Category="UI")
	class UPlayerStatBars* StatBarsInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UDataFragementsHUDWidget> DataFragmentsHUDWidget;

	UPROPERTY(BlueprintReadWrite, Category="UI")
	class UDataFragementsHUDWidget* DataFragmentsHUDInstance;

	
	virtual void BeginPlay() override;
};
