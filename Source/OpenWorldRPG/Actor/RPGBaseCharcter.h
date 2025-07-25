// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "OpenWorldRPG/BaseAttributeSet.h"
#include "OpenWorldRPG/Pickups/DataFragmentPickup.h"
#include "RPGBaseCharcter.generated.h"

UCLASS()
class OPENWORLDRPG_API ARPGBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
public:	
	ARPGBaseCharacter();

protected:

	UPROPERTY()
	TObjectPtr<UBaseAttributeSet> BaseAttributeSet;
	
	virtual void AddStartupGameplayEffects();
	virtual void AddStartupGameplayAbilities();
	void UpdateHealthStatBarDisplay();
	void UpdateStaminaStatBarDisplay();
	void UpdateManaStatBarDisplay();
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS|Defaults")
	TSubclassOf<UGameplayEffect> DefaultAttributesEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS|Defaults")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> StatBarWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> StatBarWidgetInstance;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UWidgetComponent> StatBarWidgetComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Death")
	bool bIsDead =false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Death|Loot")
	TSubclassOf<ADataFragmentPickup> DataFragmentPickupClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death|Loot")
	float DataFragmentDropValue = 10.0f;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Death|Loot")
	bool bShouldDropDataFragment = true;
	
	virtual void OnDeathGameplayEvent(const FGameplayEventData* Payload);

	UFUNCTION(BlueprintImplementableEvent, Category = "Death")
	void OnDeath();

	virtual void HandleDeath();
	virtual void SpawnDataFragmentPickup();
	
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnStaminaChanged(const FOnAttributeChangeData& Data);
	void OnManaChanged(const FOnAttributeChangeData& Data);

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};


