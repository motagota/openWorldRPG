// Fill out your copyright notice in the Description page of Project Settings.


#include "BackstepAbility.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

UBackstepAbility::UBackstepAbility()
{
    UE_LOG(LogTemp, Display, TEXT("Backstep Ability Constructor"));
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
    
     AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Movement.Backstep")));
    ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Movement.Backstep")));
    CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Movement")));
  
    InvulnerabilityTag = FGameplayTag::RequestGameplayTag(FName("State.Invulnerable"));

    BackstepTimelineDelegate.BindUFunction(this, FName("OnBackstepTimelineUpdate"));
    BackstepTimelineFinishedDelegate.BindUFunction(this, FName("OnBackstepTimelineFinished"));
}

bool UBackstepAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
    UE_LOG(LogTemp, Display, TEXT("BackstepAbility::CanActivateAbility"));
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }
    
    // Check if character is grounded
    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!Character || !Character->GetCharacterMovement()->IsMovingOnGround())
    {
        return false;
    }
    
    // Check if already in backstep or other movement abilities
    if (ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Movement.Backstep"))))
    {
        return false;
    }
    
    return true;
}

void UBackstepAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    UE_LOG(LogTemp, Display, TEXT("BackstepAbility::ActivateAbility"));
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        UE_LOG(LogTemp, Display, TEXT("BackstepAbility::ActivateAbility - CommitAbility"));
        
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
 
    OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    OwnerASC = ActorInfo->AbilitySystemComponent.Get();
    
    if (!OwnerCharacter || !OwnerASC)
    {
        UE_LOG(LogTemp, Display, TEXT("BackstepAbility::ActivateAbility - OwnerCharacter"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    
    // Check if BackstepCurve is valid
    if (!BackstepCurve)
    {
        UE_LOG(LogTemp, Error, TEXT("BackstepAbility: BackstepCurve is null! Please assign a curve asset."));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    
    OwnerCharacter->GetCharacterMovement()->DisableMovement();
       
    FVector BackstepDirection = -OwnerCharacter->GetActorForwardVector();
    StartLocation = OwnerCharacter->GetActorLocation();
    TargetLocation = StartLocation + (BackstepDirection * BackstepDistance);
    
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(OwnerCharacter);
    
    if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, TargetLocation, ECollisionChannel::ECC_WorldStatic, CollisionParams))
    {  
        TargetLocation = HitResult.Location + (HitResult.Normal * 50.0f); 
    }
  
    StartIFrames();

    if (BackstepMontage)
    {
      //  OwnerCharacter->PlayAnimMontage(BackstepMontage);
    }
      
    // Create and properly initialize the timeline
    BackstepTimeline = NewObject<UTimelineComponent>(OwnerCharacter);
    if (BackstepTimeline)
    {
        BackstepTimeline->SetTimelineLength(BackstepDuration);
        BackstepTimeline->SetLooping(false);
        
        // Add the curve interpolation - this is crucial!
        BackstepTimeline->AddInterpFloat(BackstepCurve, BackstepTimelineDelegate);
        BackstepTimeline->SetTimelineFinishedFunc(BackstepTimelineFinishedDelegate);
        
        // Register the component with the character
        BackstepTimeline->RegisterComponent();
        
        UE_LOG(LogTemp, Display, TEXT("BackstepAbility: Starting timeline with duration %f"), BackstepDuration);
        BackstepTimeline->Play();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("BackstepAbility: Failed to create timeline component"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
    }
}

void UBackstepAbility::OnBackstepTimelineUpdate(float Value)
{
    if (!OwnerCharacter) return;

    // Horizontal movement (lerp between start and target)
    FVector HorizontalLocation = FMath::Lerp(StartLocation, TargetLocation, Value);

    // Vertical movement (parabolic arc: up then down)
    float VerticalOffset = BackstepHeight * FMath::Sin(Value * PI); // Creates a smooth arc

    FVector NewLocation = HorizontalLocation + FVector(0, 0, VerticalOffset);
    OwnerCharacter->SetActorLocation(NewLocation);
}

void UBackstepAbility::OnBackstepTimelineFinished()
{
    UE_LOG(LogTemp, Display, TEXT("BackstepAbility::OnBackstepTimelineFinished"));
    if (OwnerCharacter && OwnerCharacter->GetCharacterMovement())
    {
        OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    }
    
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UBackstepAbility::StartIFrames()
{
    if (!OwnerASC)
        return;
  
    FGameplayEffectContextHandle EffectContext = OwnerASC->MakeEffectContext();
    
    
  //  FGameplayEffectSpecHandle EffectSpec = OwnerASC->MakeOutgoingSpec(
  //      UGameplayEffect::StaticClass(), 
  //      1.0f,
   //     EffectContext
   // );
    
   // if (EffectSpec.IsValid())
    //{
        // Add the invulnerability tag
   //     EffectSpec.Data->DynamicGrantedTags.AddTag(InvulnerabilityTag);
    //    IFramesEffectHandle = OwnerASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
    //}
    
    // Set timer to end I-frames
    GetWorld()->GetTimerManager().SetTimer(
        IFramesTimerHandle,
        this,
        &UBackstepAbility::EndIFrames,
        IFramesDuration,
        false
    );
}

void UBackstepAbility::EndIFrames()
{
    if (OwnerASC && IFramesEffectHandle.IsValid())
    {
   //     OwnerASC->RemoveActiveGameplayEffect(IFramesEffectHandle);
  //     IFramesEffectHandle = FActiveGameplayEffectHandle();
    }
}

void UBackstepAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
 
    if (BackstepTimeline)
    {
        BackstepTimeline->Stop();
        BackstepTimeline->DestroyComponent();
        BackstepTimeline = nullptr;
    }
    
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(IFramesTimerHandle);
    }
    EndIFrames();
    
    if (OwnerCharacter && OwnerCharacter->GetCharacterMovement())
    {
        OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    }
    
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}