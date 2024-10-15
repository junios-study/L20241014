// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidgetBase.h"
#include "Components/TextBlock.h"


void ULobbyWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	LeftTime = Cast<UTextBlock>(GetWidgetFromName(TEXT("LeftTime")));

}
