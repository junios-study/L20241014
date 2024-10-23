// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "../TPSPlayerController.h"
#include "Components/Button.h"
#include "../TPSPlayerState.h"
#include "../TPSGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "../TPSGameModeBase.h"
#include "../MyGameInstanceSubsystem.h"



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
		StartButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::ProcessStartButtonClicked);
	}

	ATPSGameStateBase* GS = Cast<ATPSGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if (IsValid(GS))
	{
		GS->OnChangePlayerCount.AddDynamic(this, &ULobbyWidgetBase::SetAliveCount);
	}
}

//성능상 문제, Tick 값 가져와야 되나?
//클래스간 의존성
//수정 방법론
//Delegate -> 함수 포인터 -> 디자인 패턴(Delegate 패턴) -> C#, java, -> BP Event Dispatcher -> Bind
void ULobbyWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//ATPSGameStateBase* GS = Cast<ATPSGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	//if (IsValid(GS))
	//{
	//	SetAliveCount(GS->PlayerCount);
	//}
}

void ULobbyWidgetBase::ProcessStartButtonClicked()
{
	GetWorld()->ServerTravel(TEXT("InGame"));
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
			UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
			UMyGameInstanceSubsystem* MySubsystem = GameInstance->GetSubsystem<UMyGameInstanceSubsystem>();
			FString Message = "";
			if (IsValid(MySubsystem))
			{
				Message = FString::Printf(TEXT("%s : %s"), *MySubsystem->UserName, *Text.ToString());
			}

			PC->C2S_SendMessage(FText::FromString(Message));
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
