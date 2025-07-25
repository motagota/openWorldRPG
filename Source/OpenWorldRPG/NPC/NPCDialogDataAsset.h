#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NPCDialogDataAsset.generated.h"

UCLASS(Blueprintable, BlueprintType)
class OPENWORLDRPG_API UNPCDialogDataAsset : public UDataAsset 
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "NPC Dialogue")
	UDataTable* DialogDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "NPC Dialogue")
	FName StartingDialogID;
		
};
