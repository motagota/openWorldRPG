// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemBase.generated.h"


/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API UItemBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	int32 MaxStackSize =1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	TSubclassOf<UGameplayEffect> EffectToApply;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	FGameplayTag RequiredTag;

	UFUNCTION(BlueprintCallable, Category = "Items")
	virtual bool Use(AActor* User);

	UFUNCTION(BlueprintCallable, Category = "Items")
	virtual bool CanUse( AActor* User) const;

	// DataAsset overrides
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
