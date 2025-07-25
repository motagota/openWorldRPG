// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
	#include "NPCDialogOptionWidget.h"

#include "NPCDialogWidget.generated.h"

class UTextBlock;
class UButton;
class ABaseNPC; // Forward declare ABaseNPC

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGoodbyePressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogOptionSelected, int32, OptionIndex);

UCLASS()
class OPENWORLDRPG_API UNPCDialogWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetDialogText(const FText& text);
	
	UFUNCTION(BlueprintCallable)
	void SetDialogOptions(const TArray<FName>& Options);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGoodbyePressed OnGoodbye;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGoodbyePressed OnNext;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDialogOptionSelected OnOptionSelected;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ShowAnim;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DialogText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SpeakerText;

	UPROPERTY(meta = (BindWidget))
	UButton* GoodByeButton;

	UPROPERTY(meta = (BindWidget))
	UButton* NextButton;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* OptionsContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialog")
	TSubclassOf<UNPCDialogOptionWidget> OptionWidgetClass;
	
protected:
	virtual void NativeConstruct() override;
	
	

	UFUNCTION()
	void HandleGoodByeClicked();
	
	UFUNCTION()
	void HandleNextClicked();

	UFUNCTION()
	void HandleOptionSelected(int32 OptionIndex);

private:

private:
	FString FullText;
	int32 CurrentCharIndex;
	FTimerHandle TypewriterTimer;

	void TypeNextLetter();

};
