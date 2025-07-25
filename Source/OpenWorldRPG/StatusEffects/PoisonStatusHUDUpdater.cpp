// Fill out your copyright notice in the Description page of Project Settings.


#include "PoisonStatusHUDUpdater.h"
#include "../HUD/PoisonStatusComponent.h"
#include "GameFramework/HUD.h"

bool APoisonStatusHUDUpdater::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	UE_LOG(LogTemp, Warning, TEXT("APoisonStatusHUDUpdater::OnExecute_Implementation"));
	NotifyHUD(MyTarget, Parameters.OriginalTag);
	return true;
}

bool APoisonStatusHUDUpdater::WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	UE_LOG(LogTemp, Warning, TEXT("APoisonStatusHUDUpdater::WhileActive_Implementation"));
	NotifyHUD(MyTarget, Parameters.OriginalTag);
	return true;
}

bool APoisonStatusHUDUpdater::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{UE_LOG(LogTemp, Warning, TEXT("APoisonStatusHUDUpdater::OnRemove_Implementation"));
	NotifyHUD(MyTarget, Parameters.OriginalTag);
	return true;
}


void APoisonStatusHUDUpdater::NotifyHUD(AActor* Target, FGameplayTag CueTag)
{
    UE_LOG(LogTemp, Warning, TEXT("APoisonStatusHUDUpdater::NotifyHUD"));
    if (APlayerController* PC = Cast<APlayerController>(Target->GetInstigatorController()))
    {
        if (UPoisonStatusComponent* PoisonComp = PC->GetHUD()->FindComponentByClass<UPoisonStatusComponent>())
        {
            UE_LOG(LogTemp, Warning, TEXT("APoisonStatusHUDUpdater::NotifyHUD poison comp"));
            PoisonComp->OnGameplayCueTriggered(FName(*CueTag.ToString()));
        }
    }
}
