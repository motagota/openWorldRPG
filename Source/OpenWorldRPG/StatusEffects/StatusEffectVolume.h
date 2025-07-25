// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffectComponent.h"
#include "StatusEffectVolume.generated.h"

UCLASS()
class OPENWORLDRPG_API AStatusEffectVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStatusEffectVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Volume")
	class UBoxComponent* TriggerVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffect")
	TSubclassOf<UGameplayEffect> BuildUpEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "StatusEffect")
	TSubclassOf<UGameplayEffect> DecayEffect;

	UPROPERTY()
	TMap<AActor*, FActiveGameplayEffectHandle> ActiveBuildUpHandles;
	
	UPROPERTY()
	TMap<AActor*, FActiveGameplayEffectHandle> ActiveDecayHandles;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
