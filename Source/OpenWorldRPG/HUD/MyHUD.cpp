// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"

#include "DataFragementsHUDWidget.h"
#include "PlayerStatBars.h"
#include "Blueprint/UserWidget.h"

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	if ( PlayerStBarsWidget)
	{
		StatBarsInstance = CreateWidget<UPlayerStatBars>( GetWorld(), PlayerStBarsWidget);

		if (StatBarsInstance)
		{
			StatBarsInstance->AddToViewport();
		}
	}

	if ( DataFragmentsHUDWidget)
	{
		DataFragmentsHUDInstance = CreateWidget<UDataFragementsHUDWidget>(GetWorld(), DataFragmentsHUDWidget);
		if (DataFragmentsHUDInstance)
		{
			DataFragmentsHUDInstance->AddToViewport();
		}
	}
}
