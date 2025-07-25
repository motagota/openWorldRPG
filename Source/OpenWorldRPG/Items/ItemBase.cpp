// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "OpenWorldRPG/OpenWorldRPGCharacter.h"


bool UItemBase::Use(AActor* User)
{
    if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(User))
    {
        UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
        if (ASC && ASC->HasMatchingGameplayTag(RequiredTag))
        {
            if (EffectToApply)
            {
                FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
                Context.AddSourceObject(this);
                ASC->ApplyGameplayEffectToSelf(EffectToApply.GetDefaultObject(), 1.0f, Context);
                return true;
            }
        }
    }
    return false;
}


bool UItemBase::CanUse( AActor* User) const
{
  
    UE_LOG(LogTemp, Warning, TEXT("UItemBase::CanUse!"));
    if (EffectToApply == nullptr) return false;

    UE_LOG(LogTemp, Warning, TEXT("UItemBase::CanUse EffectToApply is set"));
    if (RequiredTag.IsValid() && User)
    {
        UE_LOG(LogTemp, Warning, TEXT("UItemBase::CanUse EffectToApply required Tag is valid and user is set"));
        if (AOpenWorldRPGCharacter* PlayerCharacter = Cast<AOpenWorldRPGCharacter>(User))
        {
            UE_LOG(LogTemp, Log, TEXT("CanUse - Successfully cast to player character"));
        
            if (UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent())
            {
                UE_LOG(LogTemp, Log, TEXT("CanUse - Found ASC: %p"), ASC);
                            
                return ASC->HasMatchingGameplayTag(RequiredTag);
            }
        }
        return false;
    }
    return false;
}

FPrimaryAssetId UItemBase::GetPrimaryAssetId() const
{
    // Return a primary asset ID based on the class name
    return FPrimaryAssetId(FPrimaryAssetType("Item"), GetFName());
}

