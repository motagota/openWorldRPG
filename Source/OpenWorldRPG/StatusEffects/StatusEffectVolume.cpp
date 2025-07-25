// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectVolume.h"
#include "Components/BoxComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

// Sets default values
AStatusEffectVolume::AStatusEffectVolume()
{ 	
	PrimaryActorTick.bCanEverTick = false;
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	RootComponent = TriggerVolume;
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetCollisionObjectType(ECC_WorldStatic);
	TriggerVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void AStatusEffectVolume::BeginPlay()
{
	Super::BeginPlay();
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AStatusEffectVolume::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AStatusEffectVolume::OnOverlapEnd);
}

void AStatusEffectVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                         bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || !BuildUpEffect) return;

    if (UAbilitySystemComponent* ASC = OtherActor->FindComponentByClass<UAbilitySystemComponent>())
    {
        FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
        FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(BuildUpEffect, 1.0f, Context);
        if (SpecHandle.IsValid())
        {
            FActiveGameplayEffectHandle EffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
            ActiveBuildUpHandles.Add(OtherActor, EffectHandle);

            // Remove decay if it exists
            if (ActiveDecayHandles.Contains(OtherActor))
            {
                ASC->RemoveActiveGameplayEffect(ActiveDecayHandles[OtherActor]);
                ActiveDecayHandles.Remove(OtherActor);
            }
        }
    }
}

void AStatusEffectVolume::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor || !DecayEffect) return;

    if (UAbilitySystemComponent* ASC = OtherActor->FindComponentByClass<UAbilitySystemComponent>())
    {
        // Remove build-up effect
        if (ActiveBuildUpHandles.Contains(OtherActor))
        {
            ASC->RemoveActiveGameplayEffect(ActiveBuildUpHandles[OtherActor]);
            ActiveBuildUpHandles.Remove(OtherActor);
        }

        // Apply decay effect
        FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
        FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DecayEffect, 1.0f, Context);
        if (SpecHandle.IsValid())
        {
            FActiveGameplayEffectHandle EffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
            ActiveDecayHandles.Add(OtherActor, EffectHandle);
        }
    }
}
// Called every frame
void AStatusEffectVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

