// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorldRPGGameInstance.h"

#include "OpenWorldRPGCharacter.h"
#include "OpenWorldRPGSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Slate/SGameLayerManager.h"


FFastTravelLocationData::FFastTravelLocationData(): Location(FVector::ZeroVector), DisplayName(FText::GetEmpty())
{
}

void UOpenWorldRPGGameInstance::Init()
{
	Super::Init();

	LoadGame();
}

void UOpenWorldRPGGameInstance::Shutdown()
{
	Super::Shutdown();
	if ( APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0)){
		if ( AOpenWorldRPGCharacter* player = Cast<AOpenWorldRPGCharacter>(PC->GetPawn()) )
		{
			SaveGame(player->GetLastWhisperingStone()->WhisperingStoneID,player->GetActorLocation());
		}
	}
}
void UOpenWorldRPGGameInstance::SaveGame(FName GraceID, FVector PlayerLocation)
{
	auto SaveGameInstance = Cast<UOpenWorldRPGSaveGame>(UGameplayStatics::CreateSaveGameObject(UOpenWorldRPGSaveGame::StaticClass()));

	if (SaveGameInstance)
	{
		SaveGameInstance->SaveData.LastWhisperingStoneID = GraceID;
		SaveGameInstance->SaveData.LastPlayerPosition = PlayerLocation;
		SaveGameInstance->SaveData.UsedWhisperingStones = UsedWhisperingStones;
		LogWhisperingStones();
		if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameSlotName, UserIndex))
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to save game to slot"));
		}
	}
}

bool UOpenWorldRPGGameInstance::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveGameSlotName, UserIndex))
	{
		USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, UserIndex);
		if (UOpenWorldRPGSaveGame* SaveGame = Cast<UOpenWorldRPGSaveGame>(Loaded))
		{
			LastWhisperingStoneID = SaveGame->SaveData.LastWhisperingStoneID;
			LastPlayerPosition = SaveGame->SaveData.LastPlayerPosition;
			UsedWhisperingStones = SaveGame->SaveData.UsedWhisperingStones;

			LogWhisperingStones();
			return true;
		}
	}
	return false;
}

bool UOpenWorldRPGGameInstance::GetFastTravelLocationData(FName StoneID,FFastTravelLocationData& Result) const
{
	if ( FastTravelLocationsTable )
	{
		FFastTravelLocationData* FoundRow = FastTravelLocationsTable->FindRow<FFastTravelLocationData>(StoneID, TEXT("Lookup Fast Travel Location"));
		if ( FoundRow)
		{
			Result = *FoundRow;
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get fast travel location data for StoneID %s"), *StoneID.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FastTravelLocationsTable is not set in GameInstanced"));
	}
	return false;
}

void UOpenWorldRPGGameInstance::MarkWhisperingStoneAsUsed(FName StoneID)
{
	UE_LOG(LogTemp, Display, TEXT("MarkWhisperingStoneAsUsed: %s"), *StoneID.ToString());
	if (!UsedWhisperingStones.Contains(StoneID))
	{
		UE_LOG(LogTemp, Display, TEXT("MarkWhisperingStoneAsUsed - add it"));
		UsedWhisperingStones.Add(StoneID);

		LogWhisperingStones();
	}
}

bool UOpenWorldRPGGameInstance::IsWhisperingStoneUsed(FName StoneID) const
{
	UE_LOG(LogTemp, Display, TEXT("IsWhisperingStoneUsed: %s"), *StoneID.ToString());
	LogWhisperingStones();
	return UsedWhisperingStones.Contains(StoneID);
}

void UOpenWorldRPGGameInstance::LogWhisperingStones() const
{
	UE_LOG(LogTemp, Display, TEXT("----- Logging Used Whispering Stones -----"));

	if ( UsedWhisperingStones.Num() == 0 )
	{
		UE_LOG(LogTemp, Display, TEXT("No Whispering stones"));
		return;
	}

	for ( const FName& StoneID : UsedWhisperingStones )
	{
		UE_LOG(LogTemp, Display, TEXT("Used Whispering stone ID %s"), *StoneID.ToString());
	}

	UE_LOG(LogTemp, Display, TEXT("----- End of Used Whispering Stones Log -----"));

	
}