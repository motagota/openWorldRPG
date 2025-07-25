// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseNPC.h"

#include "Algo/ForEach.h"
#include "NPCDialogDataAsset.h"
#include "NPCDialogWidget.h"
#include "Components/BoxComponent.h"
#include "Components/Button.h"
#include "OpenWorldRPG/OpenWorldRPGCharacter.h"
#include "GameFramework/PlayerController.h" // Add this include


ABaseNPC::ABaseNPC(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{ 	
	PrimaryActorTick.bCanEverTick = false;
	
	dialogueTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("DialogueTrigger"));
	RootComponent = dialogueTrigger;

	dialogueTrigger->SetBoxExtent(FVector(100));
	dialogueTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABaseNPC::OnPlayerEnter);
	dialogueTrigger->OnComponentEndOverlap.AddDynamic(this, &ABaseNPC::OnPlayerExit);

	NPCInteractionPrompt = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionPrompt"));
	NPCInteractionPrompt->SetupAttachment(RootComponent);
	NPCInteractionPrompt->SetVisibility(false);

}

void ABaseNPC::BeginPlay()
{
	Super::BeginPlay();
	
}




void ABaseNPC::OnInteract()
{
    if (!DialogData || !DialogueWidgetClass) return;
    
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (AOpenWorldRPGCharacter* Player = Cast<AOpenWorldRPGCharacter>(OverlappingPlayer))
    {
        Player->DisableInput(PlayerController);
    }

    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = true;
        FInputModeUIOnly InputModeData;
        InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PlayerController->SetInputMode(InputModeData);
    }

    if (!DialogueWidgetInstance)
    {
        DialogueWidgetInstance = CreateWidget<UNPCDialogWidget>(GetWorld(), DialogueWidgetClass);
        
        if (DialogueWidgetInstance)
        {
            DialogueWidgetInstance->OnOptionSelected.AddDynamic(this, &ABaseNPC::HandleOptionSelected);

            UNPCDialogDataAsset* DialogAsset = DialogData->GetDefaultObject<UNPCDialogDataAsset>();
            if (DialogAsset)
            {
                CurrentDialogID = DialogAsset->StartingDialogID;
                FDialogData* DialogRow = GetDialogData(CurrentDialogID);

                if (DialogRow)
                {
                    DialogueWidgetInstance->AddToViewport();
                    TArray<FName> FilteredChoices = FilterAvailableChoices(DialogRow->Choices, OverlappingPlayer);
                    ShowDialogue(DialogRow->DialogText, DialogRow->Speaker, FilteredChoices, !DialogRow->NextDialogID.IsNone());
                    
                    // Process dialog actions after showing the dialog
                    ProcessDialogActions(DialogRow, OverlappingPlayer);
                    
                    DialogueWidgetInstance->OnGoodbye.AddDynamic(this, &ABaseNPC::HandleGoodbye);
                    DialogueWidgetInstance->OnNext.AddDynamic(this, &ABaseNPC::HandleNext);
                }
            }
        }
    }
}

void ABaseNPC::ShowDialogue(FText& DialogText, FText& Speaker,TArray<FName> Choices, bool bShowNextButton)
{
    if (DialogueWidgetInstance)
    {
        DialogueWidgetInstance->PlayAnimation(DialogueWidgetInstance->ShowAnim, 0.f,1);
        DialogueWidgetInstance->SetDialogText(DialogText);
        DialogueWidgetInstance->SpeakerText->SetText(Speaker);

        TArray<FName> FilteredChoices = FilterAvailableChoices(Choices, OverlappingPlayer);
        DialogueWidgetInstance->SetDialogOptions(FilteredChoices);       
        
        DialogueWidgetInstance->NextButton->SetVisibility(bShowNextButton ?ESlateVisibility::Visible : ESlateVisibility::Collapsed);
        DialogueWidgetInstance->GoodByeButton->SetVisibility(!bShowNextButton ?ESlateVisibility::Visible : ESlateVisibility::Collapsed);        
    }
}

void ABaseNPC::CloseDialogue()
{
    if (DialogueWidgetInstance)
    {
        DialogueWidgetInstance->RemoveFromParent();
        DialogueWidgetInstance = nullptr;
        
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (AOpenWorldRPGCharacter* Player = Cast<AOpenWorldRPGCharacter>(OverlappingPlayer))
        {
            Player->EnableInput(PlayerController);
        }

        if (PlayerController)
        {
            PlayerController->bShowMouseCursor = false;
            FInputModeGameOnly InputModeData;
            PlayerController->SetInputMode(InputModeData);
        }
    }
}


void ABaseNPC::OnPlayerEnter(UPrimitiveComponent* overlapped, AActor* otherActor,
                             UPrimitiveComponent* overlappedComponent, int32 otherPlayerIndex, bool bFromSweep, const FHitResult& sweepResult)
{
    if ( otherActor->IsA(AOpenWorldRPGCharacter::StaticClass()))
    {
        OverlappingPlayer = otherActor;
        NPCInteractionPrompt->SetVisibility(true);
    }
    // not a player character so ignore
}

void ABaseNPC::OnPlayerExit(UPrimitiveComponent* overlapped, AActor* otherActor, UPrimitiveComponent* otheComp,
    int32 otherBodyIndex)
{
    if (otherActor == OverlappingPlayer)
    {
        NPCInteractionPrompt->SetVisibility(false);
        OverlappingPlayer = nullptr;
    }
}

void ABaseNPC::HandleOptionSelected(int32 OptionIndex)
{
    UE_LOG(LogTemp, Log, TEXT("HandleOptionSelected - Player selected option: %d in dialog %s"),
        OptionIndex, *CurrentDialogID.ToString());

    FDialogData* CurrentDialog =  GetDialogData(CurrentDialogID);;
    if (!CurrentDialog) return;

    if ( CurrentDialog->Choices.IsValidIndex(OptionIndex))
    {
        UE_LOG(LogTemp, Log, TEXT("HandleOptionSelected -Option is valid"));
        
        FName ResponseDialogID = CurrentDialog->Choices[OptionIndex];
        if (!ResponseDialogID.IsNone())
        {
            
            CurrentDialogID = ResponseDialogID;
            FDialogData* ResponsDialog =  GetDialogData(ResponseDialogID);

            UE_LOG(LogTemp, Log, TEXT("HandleOptionSelected - Response dialog id is good"));
            if (ResponsDialog)
            {
                TArray<FName> FilteredChoices = FilterAvailableChoices(ResponsDialog->Choices, OverlappingPlayer);
                ShowDialogue(ResponsDialog->DialogText, ResponsDialog->Speaker, ResponsDialog->Choices,!ResponsDialog->NextDialogID.IsNone());
                ProcessDialogActions(ResponsDialog, OverlappingPlayer);  
            }
        }
    }
}

void ABaseNPC::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABaseNPC::HandleGoodbye()
{
    UNPCDialogDataAsset* DialogAsset = DialogData->GetDefaultObject<UNPCDialogDataAsset>();
   // ShowDialogue(DialogAsset->GoodbyeText);

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABaseNPC::CloseDialogue, 2.0f, false);
}

void ABaseNPC::HandleNext()
{
    if (!DialogData) return;

    FDialogData* CurrentDialog = GetDialogData(CurrentDialogID);
    if (!CurrentDialog) return;

    FName NextDialogId = CurrentDialog->NextDialogID;
    if (NextDialogId.IsNone())
    {
        return;
    }

    CurrentDialogID = NextDialogId;
    
    FDialogData* NextDialog = GetDialogData(CurrentDialogID);
    if (!NextDialog) return;
    TArray<FName> FilteredChoices = FilterAvailableChoices(NextDialog->Choices, OverlappingPlayer);
    ShowDialogue(NextDialog->DialogText, NextDialog->Speaker, FilteredChoices, !NextDialog->NextDialogID.IsNone());
    ProcessDialogActions(NextDialog, OverlappingPlayer);
}

FDialogData* ABaseNPC::GetDialogData(FName DialogID)
{
    if (!DialogData) return nullptr;

    UNPCDialogDataAsset* DialogAsset = DialogData->GetDefaultObject<UNPCDialogDataAsset>();
    
    if (!DialogData | !DialogAsset->DialogDataTable) return nullptr;

    static const FString ContextString(TEXT("Dialog  Context"));

    return DialogAsset->DialogDataTable->FindRow<FDialogData>(DialogID, ContextString);
    
}


bool ABaseNPC::CheckDialogPrerequisites(FDialogData* DialogRow, AActor* Player)
{
    UE_LOG(LogTemp, Log, TEXT("CheckDialogPrerequisites - Starting check for DialogRow: %s, Player: %p"), *DialogRow->ID.ToString(), Player);
    
    if (!DialogRow || !Player || !DialogRow->bHasPrerequisites) 
    {
        UE_LOG(LogTemp, Log, TEXT("CheckDialogPrerequisites - No prerequisites to check (DialogRow: %p, Player: %p, bHasPrerequisites: %d)"), 
            DialogRow, Player, DialogRow ? DialogRow->bHasPrerequisites : false);
        return true; // No prerequisites, always available
    }

    UE_LOG(LogTemp, Log, TEXT("CheckDialogPrerequisites - Has prerequisites to check"));
    
    if (AOpenWorldRPGCharacter* PlayerCharacter = Cast<AOpenWorldRPGCharacter>(Player))
    {
        UE_LOG(LogTemp, Log, TEXT("CheckDialogPrerequisites - Successfully cast to player character"));
        
        if (UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent())
        {
            UE_LOG(LogTemp, Log, TEXT("CheckDialogPrerequisites - Found ASC: %p"), ASC);
            
            // Check blocked tags - if player has any of these, don't show the option
            for (const FGameplayTag& BlockedTag : DialogRow->BlockedByTags)
            {
                UE_LOG(LogTemp, Log, TEXT("CheckDialogPrerequisites - Checking blocked tag: %s"), *BlockedTag.ToString());
                
                if (ASC->HasMatchingGameplayTag(BlockedTag))
                {
                    UE_LOG(LogTemp, Warning, TEXT("CheckDialogPrerequisites - Dialog choice blocked by tag: %s"), *BlockedTag.ToString());
                    return false;
                }
            }

            // Check required tags - player must have ALL of these
            for (const FGameplayTag& RequiredTag : DialogRow->RequiredTags)
            {
                UE_LOG(LogTemp, Log, TEXT("CheckDialogPrerequisites - Checking required tag: %s"), *RequiredTag.ToString());
                
                if (!ASC->HasMatchingGameplayTag(RequiredTag))
                {
                    UE_LOG(LogTemp, Warning, TEXT("CheckDialogPrerequisites - Dialog choice missing required tag: %s"), *RequiredTag.ToString());
                    return false;
                }
            }
            
            UE_LOG(LogTemp, Log, TEXT("CheckDialogPrerequisites - All prerequisites passed"));
            return true;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("CheckDialogPrerequisites - Could not get AbilitySystemComponent from player"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CheckDialogPrerequisites - Failed to cast player to AOpenWorldRPGCharacter"));
    }
    
    return false;
}

TArray<FName> ABaseNPC::FilterAvailableChoices(const TArray<FName>& Choices, AActor* Player)
{
    TArray<FName> FilteredChoices;

    for (const FName& ChoiceID : Choices)
    {
        FDialogData* ChoiceDialog = GetDialogData(ChoiceID);
        if (ChoiceDialog && CheckDialogPrerequisites(ChoiceDialog, Player))
        {
            FilteredChoices.Add(ChoiceID);
        }
    }

    return FilteredChoices;
}

void ABaseNPC::ProcessDialogActions(FDialogData* DialogRow, AActor* Player)
{
    UE_LOG(LogTemp, Log, TEXT("ProcessDialogActions - Starting with DialogRow: , Player: "));
    
    if (!DialogData || !Player) 
    {
        UE_LOG(LogTemp, Warning, TEXT("ProcessDialogActions - Missing required data (DialogData: , Player: )"));
        return;
    }

    // Check if this dialog should grant tags
    if (DialogRow->bShouldGrantTags && DialogRow->TagsToGrant.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("ProcessDialogActions - Attempting to grant %d tags"), DialogRow->TagsToGrant.Num());
        
        // Cast to your player character class
        if (AOpenWorldRPGCharacter* PlayerCharacter = Cast<AOpenWorldRPGCharacter>(Player))
        {
            UE_LOG(LogTemp, Log, TEXT("ProcessDialogActions - Successfully cast to player character"));
            
            // Get the Ability System Component
            if (UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent())
            {
                UE_LOG(LogTemp, Log, TEXT("ProcessDialogActions - Found ASC: "));
                
                for (const FGameplayTag& Tag : DialogRow->TagsToGrant)
                {
                    UE_LOG(LogTemp, Log, TEXT("ProcessDialogActions - Attempting to grant tag: %s"), *Tag.ToString());
                    ASC->AddLooseGameplayTag(Tag);
                    
                    // Verify the tag was actually added
                    if (ASC->HasMatchingGameplayTag(Tag))
                    {
                        UE_LOG(LogTemp, Log, TEXT("ProcessDialogActions - Successfully granted tag: %s"), *Tag.ToString());
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("ProcessDialogActions - Failed to grant tag: %s"), *Tag.ToString());
                    }
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("ProcessDialogActions - Could not get AbilitySystemComponent from player"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ProcessDialogActions - Failed to cast player to AOpenWorldRPGCharacter"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("ProcessDialogActions - No tags to grant (bShouldGrantTags: %d, TagsToGrant.Num(): %d)"), 
            DialogRow->bShouldGrantTags, DialogRow->TagsToGrant.Num());
    }
}
void ABaseNPC::Interact_Implementation(AActor* Interactor)
{
    IInteractableInterface::Interact_Implementation(Interactor);

	OverlappingPlayer = Interactor;
	OnInteract();
}
