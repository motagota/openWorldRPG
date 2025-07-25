
#include "PoisonStatusComponent.h"
#include "Blueprint/UserWidget.h"
#include "PoisonStatusWidget.h"
#include "Kismet/GameplayStatics.h"

UPoisonStatusComponent::UPoisonStatusComponent(): PoisonStatusWidget(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
	PoisonLevel = 0.0f;
}

void UPoisonStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("UPoisonStatusComponent::BeginPlay"));

	if (PoisonStatusWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPoisonStatusComponent::BeginPlay PoisonStatusWidgetClass"));
		PoisonStatusWidget = CreateWidget<UPoisonStatusWidget>(GetWorld(), PoisonStatusWidgetClass);
		if (PoisonStatusWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("UPoisonStatusComponent::BeginPlay PoisonStatusWidget"));
			PoisonStatusWidget->AddToViewport();
			PoisonStatusWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
}

void UPoisonStatusComponent::OnGameplayCueTriggered(FName CueTag)
{
    UE_LOG(LogTemp, Warning, TEXT("UPoisonStatusComponent::OnGameplayCueTriggered - CueTag: %s"), *CueTag.ToString());
	if (!PoisonStatusWidget) return;
	
	UE_LOG(LogTemp, Warning, TEXT("UPoisonStatusComponent::OnGameplayCueTriggered PoisonStatusWidget "));
	
	
	if (CueTag == "GameplayCue.GC.STATUS.PoisonBuildup")
	{
		UE_LOG(LogTemp, Warning, TEXT("UPoisonStatusComponent::OnGameplayCueTriggered PoisonBuildUp"));
		PoisonLevel = FMath::Clamp(PoisonLevel + 5.f, 0.f, PoisonMax);
		PoisonStatusWidget->UpdateBar(PoisonLevel / PoisonMax);
		PoisonStatusWidget->SetPoisonVisibility(true);
	}
	else if (CueTag == "GameplayCue.GC.STATUS.PoisonDecay")
	{
		UE_LOG(LogTemp, Warning, TEXT("UPoisonStatusComponent::OnGameplayCueTriggered PoisonDecay"));
		PoisonLevel = FMath::Clamp(PoisonLevel - 5.f, 0.f, PoisonMax);
		PoisonStatusWidget->UpdateBar(PoisonLevel / PoisonMax);
		if (PoisonLevel <= 0.f)
			PoisonStatusWidget->SetPoisonVisibility(false);
	}
	else if (CueTag == "GameplayCue.GC.STATUS.Poisoned")
	{
		UE_LOG(LogTemp, Warning, TEXT("UPoisonStatusComponent::OnGameplayCueTriggered Poisoned"));
		PoisonLevel = PoisonMax;
		PoisonStatusWidget->UpdateBar(1.0f);
		PoisonStatusWidget->PlayPoisonedEffect();
	}
}
