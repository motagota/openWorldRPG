
#pragma once
#include "../InteractableInterface.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NPCDialogDataAsset.h"
#include "NPCDialogWidget.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "BaseNPC.generated.h"


USTRUCT()
struct FDialogData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Speaker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DialogText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Choices;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextDialogID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog Actions")
	TArray<FGameplayTag> TagsToGrant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog Actions")
	bool bShouldGrantTags = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog Prerequisites")
	TArray<FGameplayTag> RequiredTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog Prerequisites")
	TArray<FGameplayTag> BlockedByTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog Prerequisites")
	bool bHasPrerequisites = false;
};

UCLASS()

class OPENWORLDRPG_API ABaseNPC : public ACharacter, public IInteractableInterface
{
	GENERATED_BODY()
	
public:
	ABaseNPC(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "NPC Dialogue | Dialog Data")
	TSubclassOf<UNPCDialogDataAsset> DialogData;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "NPC Dialogue | DialogueWidget")
	TSubclassOf<UUserWidget> DialogueWidgetClass;

	void ShowDialogue(FText& dialogText , FText& Speaker, TArray<FName> Choices, bool bShowNextButton);
	void CloseDialogue();
		
	UFUNCTION()
	void HandleGoodbye();

	UFUNCTION()
	void HandleNext();

protected:

	UPROPERTY()
	FName CurrentDialogID;

	FDialogData* GetDialogData(FName DialogID);
	void Interact_Implementation(AActor* Interactor);

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* dialogueTrigger;

	UPROPERTY(VisibleAnywhere, Category = "NPC Prompt")
	UWidgetComponent* NPCInteractionPrompt;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,  Category = "NPC Prompt")
	UNPCDialogWidget* DialogueWidgetInstance;
	
	AActor* OverlappingPlayer;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "NPC Interaction")
	void OnInteract();

	UFUNCTION()
	void OnPlayerEnter(UPrimitiveComponent* overlapped, AActor* otherActor,
			           UPrimitiveComponent* overlappedComponent, int32 otherPlayerIndex, bool bFromSweep, const FHitResult& sweepResult );

	UFUNCTION()
	void OnPlayerExit(  UPrimitiveComponent* overlapped, AActor* otherActor,
						UPrimitiveComponent* otheComp, int32 otherBodyIndex);

	UFUNCTION()
	void HandleOptionSelected(int32 OptionIndex);

	void ProcessDialogActions(FDialogData* DialogRow, AActor* Player);
	bool CheckDialogPrerequisites(FDialogData* DialogRow, AActor* Player);
	TArray<FName> FilterAvailableChoices(const TArray<FName>& Choices, AActor* Player);

public:	

	virtual void Tick(float DeltaTime) override;

};
