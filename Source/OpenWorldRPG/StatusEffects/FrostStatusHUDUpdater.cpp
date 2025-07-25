// Fill out your copyright notice in the Description page of Project Settings.


#include "FrostStatusHUDUpdater.h"

bool AFrostStatusHUDUpdater::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	UE_LOG(LogTemp, Warning, TEXT("AFrostStatusHUDUpdater::OnExecute_Implementation"));
	NotifyHUD(MyTarget, Parameters.OriginalTag);
	return true;
}

bool AFrostStatusHUDUpdater::WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	UE_LOG(LogTemp, Warning, TEXT("AFrostStatusHUDUpdater::WhileActive_Implementation"));
	NotifyHUD(MyTarget, Parameters.OriginalTag);
	return true;
}

bool AFrostStatusHUDUpdater::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{UE_LOG(LogTemp, Warning, TEXT("AFrostStatusHUDUpdater::OnRemove_Implementation"));
	NotifyHUD(MyTarget, Parameters.OriginalTag);
	return true;
}
void AFrostStatusHUDUpdater::NotifyHUD(AActor* Target, FGameplayTag CueTag)
{
	UE_LOG(LogTemp, Warning, TEXT("APoisonStatusHUDUpdater::NotifyHUD"));
	if (APlayerController* PC = Cast<APlayerController>(Target->GetInstigatorController()))
	{
		//if (UPoisonStatusComponent* PoisonComp = PC->GetHUD()->FindComponentByClass<UPoisonStatusComponent>())
		{
			UE_LOG(LogTemp, Warning, TEXT("APoisonStatusHUDUpdater::NotifyHUD poison comp"));
		//	PoisonComp->OnGameplayCueTriggered(FName(*CueTag.ToString()));
		}
	}
}
