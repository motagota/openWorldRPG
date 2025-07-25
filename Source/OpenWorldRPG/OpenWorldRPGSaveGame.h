// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OpenWorldRPGSaveGame.generated.h"

USTRUCT()
struct FOpenWorldRPGSaveGameData
{
	GENERATED_BODY()

	UPROPERTY()
	FName LastWhisperingStoneID;

	UPROPERTY()
	FVector LastPlayerPosition;
	
	UPROPERTY()
	TArray<FName> UsedWhisperingStones;
};
UCLASS()
class OPENWORLDRPG_API UOpenWorldRPGSaveGame : public USaveGame
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere)
	FOpenWorldRPGSaveGameData SaveData;

	UPROPERTY(VisibleAnywhere, Category = "SaveGame")
	FName LastWhisperingStoneID;


	UPROPERTY(VisibleAnywhere, Category = "SaveGame")
	FVector LastPlayerPosition;
	
};
