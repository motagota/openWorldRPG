// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBaseCharcter.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "OpenWorldRPG/BaseAttributeSet.h"
#include "OpenWorldRPG/EnemyHealthBarWidget.h"

// Sets default values
ARPGBaseCharacter::ARPGBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	BaseAttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("BaseAttributeSet"));

	StatBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatBarWidgetComponent"));
	StatBarWidgetComponent->SetupAttachment(GetRootComponent());
	StatBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	StatBarWidgetComponent->SetDrawAtDesiredSize(true);
	StatBarWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StatBarWidgetComponent->SetRelativeLocation(FVector(0, 0, 100));

}



// Called when the game starts or when spawned
void ARPGBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if ( AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddStartupGameplayEffects();
		AddStartupGameplayAbilities();

		static const FGameplayTag DeathTag = FGameplayTag::RequestGameplayTag("Event.Character.Death");
		AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(DeathTag).AddUObject(this, &ARPGBaseCharacter::OnDeathGameplayEvent);
		if ( BaseAttributeSet)
		{
			if ( StatBarWidgetClass)
			{
				StatBarWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), StatBarWidgetClass);
				if ( StatBarWidgetInstance)
				{
					StatBarWidgetComponent->SetWidget(StatBarWidgetInstance);

					UpdateHealthStatBarDisplay();
					UpdateStaminaStatBarDisplay();
					UpdateManaStatBarDisplay();

					AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetHealthAttribute()).AddUObject(this,&ARPGBaseCharacter::OnHealthChanged);
					AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetStaminaAttribute()).AddUObject(this,&ARPGBaseCharacter::OnStaminaChanged);
					AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetManaAttribute()).AddUObject(this,&ARPGBaseCharacter::OnManaChanged);
					
				}
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
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, *InitialStats);
			}
			*/
			
		}
	}
	
}

UAbilitySystemComponent* ARPGBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARPGBaseCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	UpdateHealthStatBarDisplay();
	UE_LOG(LogTemp, Display, TEXT("Health Changed for %s, to %.2f"), *GetName(), Data.NewValue);
}

void ARPGBaseCharacter::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	UpdateStaminaStatBarDisplay();
}

void ARPGBaseCharacter::OnManaChanged(const FOnAttributeChangeData& Data)
{
	UpdateManaStatBarDisplay();
}
void ARPGBaseCharacter::AddStartupGameplayEffects()
{
	if ( GetLocalRole() == ROLE_Authority && AbilitySystemComponent && DefaultAttributesEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributesEffect,1.0f,EffectContext);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}
void ARPGBaseCharacter::AddStartupGameplayAbilities()
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

void ARPGBaseCharacter::UpdateHealthStatBarDisplay()
{
	if ( StatBarWidgetInstance && IsValid(BaseAttributeSet))
	{
		UEnemyHealthBarWidget* tmpBar = Cast<UEnemyHealthBarWidget>(StatBarWidgetInstance);
		if ( tmpBar)
		{
			tmpBar->UpdateHealthDisplay( BaseAttributeSet->GetHealth(), BaseAttributeSet->GetMaxHealth() );

		
		}
	}
}
void ARPGBaseCharacter::UpdateStaminaStatBarDisplay()
{
	if ( StatBarWidgetInstance && IsValid(BaseAttributeSet))
	{
		UEnemyHealthBarWidget* tmpBar = Cast<UEnemyHealthBarWidget>(StatBarWidgetInstance);
		if ( tmpBar)
		{
		
			tmpBar->UpdateStaminaDisplay(BaseAttributeSet->GetStamina(), BaseAttributeSet->GetMaxStamina());

		}
	}
}
void ARPGBaseCharacter::UpdateManaStatBarDisplay()
{
	if ( StatBarWidgetInstance && IsValid(BaseAttributeSet))
	{
		UEnemyHealthBarWidget* tmpBar = Cast<UEnemyHealthBarWidget>(StatBarWidgetInstance);
		if ( tmpBar)
		{
		

			tmpBar->UpdateManaDisplay(BaseAttributeSet->GetMana(), BaseAttributeSet->GetMaxMana());
		}
	}
}
// Called every frame
void ARPGBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPGBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARPGBaseCharacter::OnDeathGameplayEvent(const FGameplayEventData* Payload)
{
	static const FGameplayTag DeathTag = FGameplayTag::RequestGameplayTag("Event.Character.Death");

	if ( Payload && Payload->EventTag == DeathTag && !bIsDead)
	{
		HandleDeath();
	}
}

void ARPGBaseCharacter::HandleDeath()
{
	UE_LOG(LogTemp, Display, TEXT("ARPGBaseCharacter::HandleDeath "));

	if (bIsDead) return;

	bIsDead = true;

	if ( bShouldDropDataFragment && DataFragmentPickupClass)
	{
		UE_LOG(LogTemp, Display, TEXT("ARPGBaseCharacter::HandleDeath- SpawnDataFragmentPickup"));
		SpawnDataFragmentPickup();
	}

	OnDeath();
}

void ARPGBaseCharacter::SpawnDataFragmentPickup()
{
	UE_LOG(LogTemp, Display, TEXT("ARPGBaseCharacter::SpawnDataFragmentPickup"));

	if (!DataFragmentPickupClass || !GetWorld()) return;

	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z += 50;

	FRotator SpawnRotation = FRotator(0, FMath::RandRange(0.0f, 360.0f), 0);
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = GetInstigator();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if ( ADataFragmentPickup* SpawnedPickup = GetWorld()->SpawnActor<ADataFragmentPickup>(DataFragmentPickupClass,SpawnLocation, SpawnRotation, SpawnParameters))
	{

		SpawnedPickup->FragmentValue = DataFragmentDropValue;		

		UE_LOG(LogTemp, Display, TEXT("%s dropped datafragment worth %.2f"), *GetName(), DataFragmentDropValue);
	}
}
