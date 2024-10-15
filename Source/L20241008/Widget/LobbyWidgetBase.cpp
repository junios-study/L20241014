// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"


void ULobbyWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	LeftTime = Cast<UTextBlock>(GetWidgetFromName(TEXT("LeftTime")));
	AliveCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("AliveCount")));
	ChatScroll = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChatScroll")));
	ChatBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ChatBox")));
	if (ChatBox)
	{
		ChatBox->OnTextCommitted.AddDynamic(this, &ULobbyWidgetBase::OnCommittedText);
	}

}

void ULobbyWidgetBase::OnCommittedText(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
	case ETextCommit::OnEnter:
		break;
	case ETextCommit::OnCleared:
		break;
	}
}
