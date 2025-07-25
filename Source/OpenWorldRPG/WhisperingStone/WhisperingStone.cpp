
#include "WhisperingStone.h"
#include "Components/BoxComponent.h"
#include "OpenWorldRPG/OpenWorldRPGCharacter.h"
#include "OpenWorldRPG/OpenWorldRPGGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "OpenWorldRPG/OpenWorldRPGPlayerController.h"

AWhisperingStone::AWhisperingStone()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetBoxExtent(FVector(100.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));

	InteractionPrompt = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionPrompt"));;
	InteractionPrompt->SetupAttachment(RootComponent);
	InteractionPrompt->SetVisibility(false);
	
	WhisperingStoneID = NAME_None;
}

void AWhisperingStone::BeginPlay()
{
	Super::BeginPlay();
	
	// Check if this stone was already used
	if (UOpenWorldRPGGameInstance* GameInstance = Cast<UOpenWorldRPGGameInstance>(GetWorld()->GetGameInstance()))
	{
		UE_LOG(LogTemp, Display, TEXT("BeginPlay - check if stone has been used"));
		bHasBeenUsed = GameInstance->IsWhisperingStoneUsed(WhisperingStoneID);

		UE_LOG(LogTemp, Display, TEXT("BeginPlay - whispering stone used %s"), bHasBeenUsed ? TEXT("true") : TEXT("false"));
	}
}

void AWhisperingStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWhisperingStone::OnTouchedByPlayer(class AOpenWorldRPGPlayerController* player)
{
	if (!bHasBeenUsed)
	{
		
		if (player)
		{
			if (UOpenWorldRPGGameInstance* GameInstance = Cast<UOpenWorldRPGGameInstance>(GetWorld()->GetGameInstance()))
			{				
				GameInstance->MarkWhisperingStoneAsUsed(WhisperingStoneID);
				bHasBeenUsed = true;
			}			
			ShowTutorialWidget(player);
		}
	}
	else
	{
		if (player)
		{
			ShowFastTravelWidget(player);
		}
	}
}

void AWhisperingStone::ShowTutorialWidget(AOpenWorldRPGPlayerController* PlayerController)
{	
	if (TutorialWidgetClass && PlayerController)
	{
		UUserWidget* TutorialWidget = CreateWidget<UUserWidget>(PlayerController, TutorialWidgetClass);
		if (TutorialWidget)
		{
			TutorialWidget->AddToViewport();			
			PlayerController->SetInputModeToUIOnly(TutorialWidget);			
		}
	}
}


void AWhisperingStone::ShowFastTravelWidget(AOpenWorldRPGPlayerController* PlayerController)
{

	if (FastTravelWidgetClass && PlayerController)
	{
		UUserWidget* FastTravelWidget = CreateWidget<UUserWidget>(PlayerController, FastTravelWidgetClass);
		if (FastTravelWidget)
		{
			FastTravelWidget->AddToViewport();
			PlayerController->SetInputModeToUIOnly(FastTravelWidget);
		}
	}
}

void AWhisperingStone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if ( AOpenWorldRPGCharacter* Player =Cast<AOpenWorldRPGCharacter>(OtherActor))
	{
		Player->HealToFull();
		Player->SetWhisperingStoneGrace(this);

		InteractionPrompt->SetVisibility(true);
		bPlayerInRange = true;
	}	
}

void AWhisperingStone::NotifyActorEndOverlap(AActor* OtherActor)
{
	if ( AOpenWorldRPGCharacter* Player =Cast<AOpenWorldRPGCharacter>(OtherActor))
	{
		bPlayerInRange = false;
		InteractionPrompt->SetVisibility(false);
	}
}
