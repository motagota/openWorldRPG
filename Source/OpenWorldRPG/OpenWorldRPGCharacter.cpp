// Copyright Epic Games, Inc. All Rights Reserved.

#include "OpenWorldRPGCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "OpenWorldRPGGameInstance.h"
#include "RPGPawnComponent.h" // Added include
#include "Actor/DataFragmentManagerComponent.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "HUD/MyHUD.h"


//////////////////////////////////////////////////////////////////////////
// AOpenWorldRPGCharacter

AOpenWorldRPGCharacter::AOpenWorldRPGCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Log, TEXT("Creating OpenWorldRPGCharacter: %s"), *GetNameSafe(this));

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm	

	PrimaryActorTick.bCanEverTick = true;
	PawnExtComponent = CreateDefaultSubobject<URPGPawnComponent>(TEXT("PawnExtComponent"));	


	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	BaseAttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("BaseAttributeSet"));
	SetNetUpdateFrequency(100.0f);	

	DataFragmentManager = CreateDefaultSubobject<UDataFragmentManagerComponent>(TEXT("DataFragmentManager"));

	
	UE_LOG(LogTemp, Log, TEXT("finished OpenWorldRPGCharacter: %s"), *GetNameSafe(this));

	

}
void AOpenWorldRPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UOpenWorldRPGGameInstance* GameInstance = Cast<UOpenWorldRPGGameInstance>(GetWorld()->GetGameInstance()))
	{
		UE_LOG(LogTemp, Display, TEXT("AOpenWorldRPGCharacter::BeginPlay - load save game data"));
		SetActorLocation(GameInstance->LastPlayerPosition);
		SetWhisperingStoneGraceID(GameInstance->LastWhisperingStoneID);
	}
	
	// save game every 5 mins
	GetWorldTimerManager().SetTimer(AutoSaveTimer, this, &AOpenWorldRPGCharacter::AutoSave, 300.f, true);
	
	if ( AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddStartupGameplayEffects();
		AddStartupGameplayAbilities();

		if ( BaseAttributeSet)
		{
			APlayerController * PC = Cast<APlayerController>(GetController());

			if ( PC)
			{
				AMyHUD* myHUD = Cast<AMyHUD>(PC->GetHUD());

				if (myHUD)
				{					
					StatBarWidgetInstance = myHUD->StatBarsInstance;
					DataFragmentWidgetInstance = myHUD->DataFragmentsHUDInstance;
				}


		
			}
			if ( StatBarWidgetInstance)
			{
				UpdateHealthStatBarDisplay();
				UpdateManaStatBarDisplay();
				UpdateStaminaStatBarDisplay();

				AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetHealthAttribute()).AddUObject(this,&AOpenWorldRPGCharacter::OnHealthChanged);
				AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetManaAttribute()).AddUObject(this,&AOpenWorldRPGCharacter::OnManaChanged);
				AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetStaminaAttribute()).AddUObject(this,&AOpenWorldRPGCharacter::OnStaminaChanged);
			}

			if ( DataFragmentWidgetInstance)
			{
				DataFragmentWidgetInstance->Init();
			}

			/*
			FString InitialStats = FString("--- Initial Stats ---");
			InitialStats += FString::Printf(TEXT("\nHealth: %.2f / %.2f"), BaseAttributeSet->GetHealth(), BaseAttributeSet->GetMaxHealth());
			InitialStats += FString::Printf(TEXT("\nStamina: %.2f / %.2f"), BaseAttributeSet->GetStamina(), BaseAttributeSet->GetMaxStamina());
			InitialStats += FString::Printf(TEXT("\nMana: %.2f / %.2f"), BaseAttributeSet->GetMana(), BaseAttributeSet->GetMaxMana());
			InitialStats += FString::Printf(TEXT("\nAttack Power: %.2f"), BaseAttributeSet->GetAttackPower());
			InitialStats += FString::Printf(TEXT("\nDefense Power: %.2f"), BaseAttributeSet->GetDefensePower());

			UE_LOG(LogTemp, Display, TEXT("Initial Stats:%s"), *InitialStats);

			if ( GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, *InitialStats);
			}
			*/
		}

	
	}

}



//////////////////////////////////////////////////////////////////////////
// Input

void AOpenWorldRPGCharacter::OnBackstepPressed()
{
	bBackstepInputHeld = true;
    	
	GetWorld()->GetTimerManager().SetTimer(
		BackstepCheckTimer,
		this,
		&AOpenWorldRPGCharacter::TriggerBackstep,
		0.1f, 
		false
	);
}
void AOpenWorldRPGCharacter::OnBackstepReleased()
{
	bBackstepInputHeld = false;

	GetWorld()->GetTimerManager().ClearTimer(BackstepCheckTimer);
}

bool AOpenWorldRPGCharacter::HasDirectionalInput() const
{
	UE_LOG(LogTemp, Display, TEXT("HasDirectionalInput"));
	if (!GetCharacterMovement())
		return false;

	FVector MovementInput = GetCharacterMovement()->GetLastInputVector();
	return MovementInput.SizeSquared() > 0.01f; 
}
void AOpenWorldRPGCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}


void AOpenWorldRPGCharacter::TriggerBackstep()
{
	UE_LOG(LogTemp, Display, TEXT("Trigger Backstep"));
	if (bBackstepInputHeld && !HasDirectionalInput() && AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Display, TEXT("Trigger Backstep - try activate "));
		FGameplayTagContainer TriggerTags;
		AbilitySystemComponent->TryActivateAbilitiesByTag(
			FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Movement.Backstep"))),
			true
		);
	}
}
void AOpenWorldRPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOpenWorldRPGCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOpenWorldRPGCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AOpenWorldRPGCharacter::AddStartupGameplayEffects()
{

	if ( GetLocalRole() == ROLE_Authority && AbilitySystemComponent && DefaultAttributesEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributesEffect,1.0f,EffectContext);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void AOpenWorldRPGCharacter::AddStartupGameplayAbilities()
{
	if ( GetLocalRole() == ROLE_Authority)
	{
		for ( TSubclassOf<UGameplayAbility>& StartupAbility : DefaultAbilities )
		{
			if (StartupAbility)
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(StartupAbility,1);
				AbilitySystemComponent->GiveAbility(AbilitySpec);
			}
		}
	}
}

void AOpenWorldRPGCharacter::UpdateHealthStatBarDisplay()
{
	if ( StatBarWidgetInstance)
	{
		StatBarWidgetInstance->UpdateHealthDisplay(BaseAttributeSet->GetHealth(), BaseAttributeSet->GetMaxHealth() );
	}
}

void AOpenWorldRPGCharacter::UpdateStaminaStatBarDisplay()
{
	if ( StatBarWidgetInstance)
	{
		StatBarWidgetInstance->UpdateStaminaDisplay(BaseAttributeSet->GetStamina(), BaseAttributeSet->GetMaxStamina() );
	}
}

void AOpenWorldRPGCharacter::UpdateManaStatBarDisplay()
{
	if ( StatBarWidgetInstance)
	{
		StatBarWidgetInstance->UpdateManaDisplay(BaseAttributeSet->GetMana(), BaseAttributeSet->GetMaxMana() );
	}
}

void AOpenWorldRPGCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AOpenWorldRPGCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
void AOpenWorldRPGCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AOpenWorldRPGCharacter::HealToFull()
{
	BaseAttributeSet->SetHealth(BaseAttributeSet->GetMaxHealth());
	BaseAttributeSet->SetStamina(BaseAttributeSet->GetMaxStamina());
	BaseAttributeSet->SetMana(BaseAttributeSet->GetMaxMana());
}

void AOpenWorldRPGCharacter::SetWhisperingStoneGrace(AWhisperingStone* WhisperingStone)
{
	LastWhisperingStone = WhisperingStone;
}

void AOpenWorldRPGCharacter::SetWhisperingStoneGraceID(FName WhisperingStoneID)
{
	LastWhisperingStoneID = WhisperingStoneID;
}

AWhisperingStone* AOpenWorldRPGCharacter::GetLastWhisperingStone() const
{
	return LastWhisperingStone;
}

void AOpenWorldRPGCharacter::AutoSave()
{
	if ( UOpenWorldRPGGameInstance* GI = Cast<UOpenWorldRPGGameInstance>(GetGameInstance()))
	{
		if (GetLastWhisperingStone() )
		{
			GI->SaveGame(GetLastWhisperingStone()->WhisperingStoneID, GetActorLocation());
		}
		else
		{
			GI->SaveGame("", GetActorLocation());
		}
		UE_LOG(LogTemp, Display, TEXT("AOpenWorldRPGCharacter::AutoSave Auto-saved."));
	}
}


void AOpenWorldRPGCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	UpdateHealthStatBarDisplay();
}
void AOpenWorldRPGCharacter::OnManaChanged(const FOnAttributeChangeData& Data)
{
	UpdateManaStatBarDisplay();
}

void AOpenWorldRPGCharacter::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	UpdateStaminaStatBarDisplay();
}

UAbilitySystemComponent* AOpenWorldRPGCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}



