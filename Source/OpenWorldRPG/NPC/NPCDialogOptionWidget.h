// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCDialogOptionWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptionSelected, int32, OptionIndex);

UCLASS()
class OPENWORLDRPG_API UNPCDialogOptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOptionText(const FName& Text, int32 Index);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnOptionSelected OnOptionSelected;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* OptionButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* OptionText;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleOptionClicked();

private:
	int32 MyIndex;
};