#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DataFragmentManagerComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDataFragmentsChanged, int32, NewAmount, int32 , OldAmount);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPENWORLDRPG_API UDataFragmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDataFragmentManagerComponent();

	UPROPERTY(BlueprintAssignable, Category="DataFragments")
	FOnDataFragmentsChanged OnDataFragmentsChanged;

private:
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;
	void InitializeAttributes();
	void BindToAttributeChanges();

	void OnDataFragmentsAttributeChanged(const FOnAttributeChangeData& Data);

	
protected:
	virtual void BeginPlay() override;
	

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure, Category="DataFragments")
	int32 GetCurrentDataFragments() const;

	UFUNCTION(BlueprintCallable, Category="DataFragments")
	bool SpendDataFragments(int32 Amount);

	UFUNCTION(BlueprintCallable, Category="DataFragments")
	void AddDataFragments(int32 Amount);

	UFUNCTION(BlueprintCallable, Category="DataFragments")
	bool CanAfford(int32 Cost) const;
};
