// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "BackstepAbility.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API UBackstepAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
    UBackstepAbility();

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

    // Backstep properties
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Backstep")
    float BackstepDistance = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Backstep")
    float BackstepHeight = 100.0f; 

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Backstep")
    float BackstepDuration = 0.6f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Backstep")
    UCurveFloat* BackstepCurve;
    
    // I-frames properties
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IFrames")
    float IFramesDuration = 0.4f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IFrames")
    FGameplayTag InvulnerabilityTag;
    
    // Animation
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    UAnimMontage* BackstepMontage;

private:
    UFUNCTION()
    void OnBackstepTimelineUpdate(float Value);
    
    UFUNCTION()
    void OnBackstepTimelineFinished();
    void StartIFrames();

    UFUNCTION()
    void EndIFrames();

    // Timeline for smooth movement
    UPROPERTY()
    UTimelineComponent* BackstepTimeline;
    
    // Movement tracking
    FVector StartLocation;
    FVector TargetLocation;
    
    // References
    UPROPERTY()
    ACharacter* OwnerCharacter;
    
    UPROPERTY()
    UAbilitySystemComponent* OwnerASC;
    
    // I-frames effect handle
    FActiveGameplayEffectHandle IFramesEffectHandle;
    
    // Delegates
    FOnTimelineFloat BackstepTimelineDelegate;
    FOnTimelineEvent BackstepTimelineFinishedDelegate;
    
    // Timer for I-frames
    FTimerHandle IFramesTimerHandle;
};
