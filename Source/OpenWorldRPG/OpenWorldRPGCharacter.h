// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DataFragmentManagerComponent.h"
#include "Actor/RPGBaseCharcter.h"
#include "GameFramework/Character.h"
#include "HUD/DataFragementsHUDWidget.h"
#include "HUD/PlayerStatBars.h"
#include "Logging/LogMacros.h"
#include "WhisperingStone/WhisperingStone.h"
#include "OpenWorldRPGCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class URPGPawnComponent;        // Forward declaration

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AOpenWorldRPGCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	AOpenWorldRPGCharacter(const FObjectInitializer& ObjectInitializer);
	void BeginPlay();

	
	virtual void PostInitializeComponents() override;

	void HealToFull();
	void SetWhisperingStoneGrace(AWhisperingStone* WhisperingStone);
	void SetWhisperingStoneGraceID(FName WhisperingStoneID);

	AWhisperingStone* GetLastWhisperingStone() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS|Defaults")
	TSubclassOf<UGameplayEffect> DefaultAttributesEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS|Defaults")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
private:

	FTimerHandle AutoSaveTimer;

	UFUNCTION(BlueprintCallable, Category = "AutoSave")
	void AutoSave();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RPG|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URPGPawnComponent> PawnExtComponent;

	UPROPERTY()
	AWhisperingStone* LastWhisperingStone;

	UPROPERTY()
	FName LastWhisperingStoneID;

	bool bBackstepInputHeld = false;    

	FTimerHandle BackstepCheckTimer;    

	bool HasDirectionalInput() const;
  
	void TriggerBackstep();
protected:

	UPROPERTY()
	TObjectPtr<UBaseAttributeSet> BaseAttributeSet;

	virtual void AddStartupGameplayEffects();
	virtual void AddStartupGameplayAbilities();
	void UpdateHealthStatBarDisplay();
	void UpdateStaminaStatBarDisplay();
	void UpdateManaStatBarDisplay();	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UDataFragmentManagerComponent> DataFragmentManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<class UBackstepAbility> BackstepAilityClass;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void OnBackstepPressed();
    
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void OnBackstepReleased();
	
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY()
	TObjectPtr<UPlayerStatBars> StatBarWidgetInstance;

	UPROPERTY()
	TObjectPtr <UDataFragementsHUDWidget> DataFragmentWidgetInstance;

	void OnHealthChanged(const FOnAttributeChangeData& Data);	
	void OnManaChanged(const FOnAttributeChangeData& Data);
	void OnStaminaChanged(const FOnAttributeChangeData& Data);
	

public:
	UAbilitySystemComponent* GetAbilitySystemComponent() const;
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};


