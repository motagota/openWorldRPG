// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "RPGPawnComponent.generated.h"

class UGameFrameworkComponentManager;
/**
 * 
 */
class URPGAbilitySystemComponent;
DECLARE_LOG_CATEGORY_EXTERN(LogRPGPawnComponent, Log, All);

UCLASS()
class OPENWORLDRPG_API URPGPawnComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
public:
	URPGPawnComponent(const FObjectInitializer& ObjectInitializer);
	
	/** Register with the OnAbilitySystemInitialized delegate and broadcast if our pawn has been registered with the ability system component */
	void OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);
	UFUNCTION(BlueprintPure, Category = "Lyra|Pawn")
	static URPGPawnComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<URPGPawnComponent>() : nullptr); }

	/** Register with the OnAbilitySystemUninitialized delegate fired when our pawn is removed as the ability system's avatar actor */
	void OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate);

	void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	                           FGameplayTag DesiredState);

protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
};
