

#include "DataFragmentManagerComponent.h"

#include "AbilitySystemComponent.h"
#include "OpenWorldRPG/Attributes/DataFragmentsAttributeSet.h"

UDataFragmentManagerComponent::UDataFragmentManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDataFragmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		 AbilitySystemComponent = OwnerPawn->FindComponentByClass<UAbilitySystemComponent>();
	}

	if ( AbilitySystemComponent)
	{
		InitializeAttributes();
		BindToAttributeChanges();
	}
	
}

void UDataFragmentManagerComponent::InitializeAttributes()
{
	if (!AbilitySystemComponent) return;

	const UDataFragmentsAttributeSet* DataFragmentSet = AbilitySystemComponent->GetSet<UDataFragmentsAttributeSet>();
	if (!DataFragmentSet)
	{
		UDataFragmentsAttributeSet* newDataFrgmentSet = NewObject<UDataFragmentsAttributeSet>(AbilitySystemComponent);
		AbilitySystemComponent->AddAttributeSetSubobject(newDataFrgmentSet);
	}
}

void UDataFragmentManagerComponent::BindToAttributeChanges()
{
	if (!AbilitySystemComponent) return;

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UDataFragmentsAttributeSet::GetDataFragmentsAttribute()).AddUObject(this, &UDataFragmentManagerComponent::OnDataFragmentsAttributeChanged);
}

void UDataFragmentManagerComponent::OnDataFragmentsAttributeChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Display, TEXT("DataFragmentManagerComponent::OnDataFragmentsAttributeChanged"));
	int32 NewAmount = FMath::RoundToInt(Data.NewValue);
	int32 OldAmount = FMath::RoundToInt(Data.OldValue);
	
	OnDataFragmentsChanged.Broadcast(NewAmount, OldAmount);
}


void UDataFragmentManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

int UDataFragmentManagerComponent::GetCurrentDataFragments() const
{
	if (!AbilitySystemComponent) return 0;

	return FMath::RoundToInt(AbilitySystemComponent->GetNumericAttribute(UDataFragmentsAttributeSet::GetDataFragmentsAttribute()));
}

void UDataFragmentManagerComponent::AddDataFragments(int32 Amount)
{
	if (!AbilitySystemComponent) return;
	AbilitySystemComponent->ApplyModToAttribute(UDataFragmentsAttributeSet::GetDataFragmentsAttribute(), EGameplayModOp::Additive, Amount);
}

bool UDataFragmentManagerComponent::SpendDataFragments(int32 Amount)
{
	if ( !CanAfford(Amount)) return false;

	AddDataFragments(-Amount);
	return true;
}

bool UDataFragmentManagerComponent::CanAfford(int32 Cost) const
{
	return GetCurrentDataFragments() >= Cost;
}
