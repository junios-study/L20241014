// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"


void UTitleWidgetBase::NativeConstruct()
{
	StartServerButton = Cast<UButton>(GetWidgetFromName(TEXT("StartServerButton")));
	ConnectButton = Cast<UButton>(GetWidgetFromName(TEXT("ConnectButton")));
	ServerIP = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ServerIP")));
	if (StartServerButton)
	{
		StartServerButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::ProcessStartServerButtonClick);
	}

	if (ConnectButton)
	{
		ConnectButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::ProcessConnectButtonClick);
	}


}

void UTitleWidgetBase::ProcessStartServerButtonClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"), true, TEXT("listen"));
}

void UTitleWidgetBase::ProcessConnectButtonClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*ServerIP->GetText().ToString()));
}
