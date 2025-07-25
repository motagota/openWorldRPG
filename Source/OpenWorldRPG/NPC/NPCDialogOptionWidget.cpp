
#include "NPCDialogOptionWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UNPCDialogOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (OptionButton)
	{
		OptionButton->OnClicked.AddDynamic(this, &UNPCDialogOptionWidget::HandleOptionClicked);
	}
}

void UNPCDialogOptionWidget::SetOptionText(const FName& Text, int32 Index)
{
	if (OptionText)
	{
		OptionText->SetText(FText::FromName(Text));
	}
	MyIndex = Index;
}

void UNPCDialogOptionWidget::HandleOptionClicked()
{
	OnOptionSelected.Broadcast(MyIndex);
}