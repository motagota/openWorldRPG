// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPawnComponent.h"
#include "RPGPlayerState.h"

DEFINE_LOG_CATEGORY(LogRPGPawnComponent);

class ARPGPlayerState;
class UActorComponent;

URPGPawnComponent::URPGPawnComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

}

void URPGPawnComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for changes to all features
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);
	
	// Notifies state manager that we have spawned, then try rest of default initialization
//	ensure(TryToChangeInitState(LyraGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void URPGPawnComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}
void URPGPawnComponent::OnRegister()
{
	Super::OnRegister();

	const APawn* Pawn = GetPawn<APawn>();
	ensureAlwaysMsgf((Pawn != nullptr), TEXT("LyraPawnExtensionComponent on [%s] can only be added to Pawn actors."), *GetNameSafe(GetOwner()));

	TArray<UActorComponent*> PawnExtensionComponents;
	Pawn->GetComponents(URPGPawnComponent::StaticClass(), PawnExtensionComponents);
	ensureAlwaysMsgf((PawnExtensionComponents.Num() == 1), TEXT("Only one LyraPawnExtensionComponent should exist on [%s]."), *GetNameSafe(GetOwner()));

	// Register with the init state system early, this will only work if this is a game world
	RegisterInitStateFeature();
}
void URPGPawnComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	UE_LOG(LogRPGPawnComponent, Log,TEXT("Setting up ASC"));

	//if (CurrentState == LyraGameplayTags::InitState_DataAvailable && DesiredState == LyraGameplayTags::InitState_DataInitialized)
{
	APawn* Pawn = GetPawn<APawn>();
	ARPGPlayerState* LyraPS = GetPlayerState<ARPGPlayerState>();
	if (!ensure(Pawn && LyraPS))
	{
		return;
	}

	//const ULyraPawnData* PawnData = nullptr;

//if (ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
	//	PawnData = PawnExtComp->GetPawnData<ULyraPawnData>();

		// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
		// The ability system component and attribute sets live on the player state.
	//	PawnExtComp->InitializeAbilitySystem(LyraPS->GetLyraAbilitySystemComponent(), LyraPS);
	//	InitializeAbilitySystem(AbilitySystemComponent, LyraPS);
		
	}

	//if (ALyraPlayerController* LyraPC = GetController<ALyraPlayerController>())
	{
		if (Pawn->InputComponent != nullptr)
		{
		//	InitializePlayerInput(Pawn->InputComponent);
		}
	}

	// Hook up the delegate for all pawns, in case we spectate later
//	if (PawnData)
	{
	//	if (ULyraCameraComponent* CameraComponent = ULyraCameraComponent::FindCameraComponent(Pawn))
		{
	//		CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
		}
	}
}
}
