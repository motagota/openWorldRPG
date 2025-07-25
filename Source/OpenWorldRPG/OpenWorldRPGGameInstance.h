// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OpenWorldRPGSaveGame.h"
#include "Engine/GameInstance.h"
#include "OpenWorldRPGGameInstance.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FFastTravelLocationData: public FTableRowBase
{
	GENERATED_BODY()
public:
	FFastTravelLocationData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fast Travel")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fast Travel")
	FText DisplayName;
};

UCLASS()
class OPENWORLDRPG_API UOpenWorldRPGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	void Shutdown();

	UFUNCTION(BlueprintCallable)
	void SaveGame(FName GraceID, FVector PlayerLocation);
	UFUNCTION(BlueprintCallable)
	bool LoadGame();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Game Data| Fast Travel")
	UDataTable* FastTravelLocationsTable;

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
	TArray<FName> UsedWhisperingStones;

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
	FName LastWhisperingStoneID;
	
	UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
	FVector LastPlayerPosition;

	UFUNCTION(BlueprintCallable, Category="Game Data|Fast Travel")
	bool GetFastTravelLocationData(FName StoneID, FFastTravelLocationData& Result) const;
public:
	void MarkWhisperingStoneAsUsed(FName StoneID);
	bool IsWhisperingStoneUsed(FName StoneID) const;
	void LogWhisperingStones() const;
	FString SaveGameSlotName = TEXT("OpenWorldSaveGameSlot");
	uint32 UserIndex = 0;
	
};
