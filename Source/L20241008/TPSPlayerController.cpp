// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerController.h"
#include "Widget/LobbyWidgetBase.h"

void ATPSPlayerController::BeginPlay()
{
	Super::BeginPlay();


	//#include
	FSoftClassPath LobbyWidgetClassPath(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UMG/WBP_Lobby.WBP_Lobby_C'"));

	if (IsLocalPlayerController())
	{
		UClass* WidgetClass = LobbyWidgetClassPath.TryLoadClass<ULobbyWidgetBase>();
		if (IsValid(WidgetClass))
		{
			LobbyWidget = CreateWidget<ULobbyWidgetBase>(this, WidgetClass);
			if (IsValid(LobbyWidget))
			{
				LobbyWidget->AddToViewport();
			}
		}
	}
}

bool ATPSPlayerController::C2S_SendMessage_Validate(const FText& Messsage)
{
	return true;
}

//call client , execute server
void ATPSPlayerController::C2S_SendMessage_Implementation(FText const& Message)
{
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter )
	{
		ATPSPlayerController* PC = Cast< ATPSPlayerController>(*Iter);
		if (IsValid(PC))
		{
			PC->S2C_AddMessage(Message);
		}
	}
}

//call server, execute client
void ATPSPlayerController::S2C_AddMessage_Implementation(const FText& Messsage)
{
	LobbyWidget->AddChatMessage(Messsage);
}
