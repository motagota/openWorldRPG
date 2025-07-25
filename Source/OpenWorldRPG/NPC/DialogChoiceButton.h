// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogChoiceButton.generated.h"
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API UDialogChoiceButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* ChoiceButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChoiceText;

	void Setup(FText ChoiceText, FName ChoiceID);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChoiceButtonClicked, FName, ChoiceID);
	UPROPERTY(BlueprintAssignable)
	FOnChoiceButtonClicked OnChoiceButtonClicked;

private:
	UFUNCTION()
	void HandleClick();

	FName MyChoiceID;
	
};
