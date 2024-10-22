﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "../TPSPlayerController.h"
#include "Components/Button.h"
#include "../TPSPlayerState.h"



void ULobbyWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	LeftTime = Cast<UTextBlock>(GetWidgetFromName(TEXT("LeftTime")));
	AliveCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("AliveCount")));
	ChatScroll = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChatScroll")));
	ChatBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ChatBox")));
	ReadyButton = Cast<UButton>(GetWidgetFromName(TEXT("ReadyButton")));
	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));

	if (ChatBox)
	{
		ChatBox->OnTextCommitted.AddDynamic(this, &ULobbyWidgetBase::OnCommittedText);
	}
	if (ReadyButton)
	{
		ReadyButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::ProcessClicked);
	}
	if (StartButton)
	{
		//StartButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::ProcessClicked);
	}
}

void ULobbyWidgetBase::ProcessClicked()
{
	ATPSPlayerState* PS = GetOwningPlayerState<ATPSPlayerState>();
	ATPSPlayerController* PC = GetOwningPlayer<ATPSPlayerController>();
	if (IsValid(PS) && IsValid(PC))
	{
		PC->C2S_SendReadyState(!PS->bReadyState);
		UE_LOG(LogTemp, Warning, TEXT("State %d"), PS->bReadyState);
	}
}

void ULobbyWidgetBase::OnCommittedText(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
	case ETextCommit::OnEnter:
	{
		ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetOwningPlayer());
		if (IsValid(PC))
		{
			PC->C2S_SendMessage(Text);
		}
		ChatBox->SetText(FText::FromString(TEXT("")));
		break;
	}
	case ETextCommit::OnCleared:
		ChatBox->SetUserFocus(GetOwningPlayer());
		break;
	}
}

void ULobbyWidgetBase::SetLeftTime(FString NewLeftTime)
{
	FString Temp = FString::Printf(TEXT("%s초 남았습니다."), *NewLeftTime);
	LeftTime->SetText(FText::FromString(Temp));
}

void ULobbyWidgetBase::SetAliveCount(uint32 NewAliveCount)
{
	FString Temp = FString::Printf(TEXT("%d명"), NewAliveCount);

	AliveCount->SetText(FText::FromString(Temp));
}

void ULobbyWidgetBase::AddChatMessage(FText AddMessage)
{
	UTextBlock* NewText = NewObject<UTextBlock>(ChatScroll);
	if (IsValid(NewText))
	{
		NewText->SetText(AddMessage);
		FSlateFontInfo NewFont = NewText->GetFont();
		NewFont.Size = 18.0f;
		NewText->SetFont(NewFont);
		ChatScroll->AddChild(NewText);
		ChatScroll->ScrollToEnd();
	}
}

void ULobbyWidgetBase::ShowStartrButton(bool Server)
{
	if (Server)
	{
		ReadyButton->SetVisibility(ESlateVisibility::Collapsed);
		StartButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ReadyButton->SetVisibility(ESlateVisibility::Visible);
		StartButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}
