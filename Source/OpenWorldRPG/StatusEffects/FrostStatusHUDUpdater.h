// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "FrostStatusHUDUpdater.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API AFrostStatusHUDUpdater : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()
public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

protected:
	void NotifyHUD(AActor* Target, FGameplayTag CueTag);
};
