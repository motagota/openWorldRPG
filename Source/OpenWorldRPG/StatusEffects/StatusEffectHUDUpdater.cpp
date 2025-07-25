// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectHUDUpdater.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/HUD.h"
#include "OpenWorldRPG/HUD/StatusEffectComponent.h"

bool AStatusEffectHUDUpdater::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	UE_LOG(LogTemp, Warning, TEXT("AStatusEffectHUDUpdater::OnExecute_Implementation"));
	UE_LOG(LogTemp, Warning, TEXT("AStatusEffectHUDUpdater::OnExecute_Implementation original tag %s") ,*Parameters.OriginalTag.ToString());
	UE_LOG(LogTemp, Warning, TEXT("AStatusEffectHUDUpdater::OnExecute_Implementation raw mag %f") ,Parameters.RawMagnitude);
	UE_LOG(LogTemp, Warning, TEXT("AStatusEffectHUDUpdater::OnExecute_Implementation norm mag %f") ,Parameters.NormalizedMagnitude);
	UE_LOG(LogTemp, Warning, TEXT("AStatusEffectHUDUpdater::OnExecute_Implementation matched tag %s") ,*Parameters.MatchedTagName.ToString());

	NotifyHUD(MyTarget, Parameters);
	return true;
}

bool AStatusEffectHUDUpdater::WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	UE_LOG(LogTemp, Warning, TEXT("AStatusEffectHUDUpdater::WhileActive_Implementation"));
	NotifyHUD(MyTarget, Parameters);
	return true;
}

bool AStatusEffectHUDUpdater::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{UE_LOG(LogTemp, Warning, TEXT("AStatusEffectHUDUpdater::OnRemove_Implementation"));
	NotifyHUD(MyTarget, Parameters);
	return true;
}
void AStatusEffectHUDUpdater::NotifyHUD(AActor* Target,  const FGameplayCueParameters& Parameters)
{
	UE_LOG(LogTemp, Warning, TEXT("AStatusEffectHUDUpdater::NotifyHUD"));
	if (APlayerController* PC = Cast<APlayerController>(Target->GetInstigatorController()))
	{
		if (UStatusEffectComponent* StatusEffectComp = PC->GetHUD()->FindComponentByClass<UStatusEffectComponent>())
		{
			UE_LOG(LogTemp, Warning, TEXT("AStatusEffectHUDUpdater::NotifyHUD StatusEffectComp"));
			
			StatusEffectComp->OnGameplayCueTriggered(FName(Parameters.OriginalTag.ToString()), Parameters.NormalizedMagnitude );			
			
		}
	}
}