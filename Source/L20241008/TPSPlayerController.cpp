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
