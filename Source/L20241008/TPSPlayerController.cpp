// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerController.h"
#include "Widget/LobbyWidgetBase.h"
#include "TPSPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"


void ATPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FString Temp = FString::Printf(TEXT("%s %d %d"), *GetName(), GetLocalRole(), GetRemoteRole());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Temp);

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
				SetInputMode(FInputModeGameAndUI());
				//bShowMouseCursor = true;
				SetShowMouseCursor(true);

				//Standalone, ListenServer, DedicatedServer 코드 잘 작동하게 설계 해야됨
				if (GetLocalRole() == ENetRole::ROLE_Authority && GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
				{
					//Server, PC
					//LobbyWidget->ShowStartrButton(HasAuthority());
					LobbyWidget->ShowStartrButton(true);
				}
				else
				{
					LobbyWidget->ShowStartrButton(false);
				}
				
				//LobbyWidget->ShowStartrButton(UKismetSystemLibrary::IsServer(GetWorld()));
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

void ATPSPlayerController::C2S_SendReadyState_Implementation(bool NewState)
{
	ATPSPlayerState* PS = GetPlayerState<ATPSPlayerState>();
	if (IsValid(PS))
	{
		//Server PlayerState 값변경, 자동 복제, Notify X
		PS->bReadyState = NewState;
		//Server
		PS->OnRep_ReadyState();
		
		uint8 TotalReadCount = 0;
		TArray<AActor*> FindActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATPSPlayerState::StaticClass(), FindActors);
		for (auto FindActor : FindActors)
		{
			ATPSPlayerState* FindPS = Cast<ATPSPlayerState>(FindActor);
			if (IsValid(FindPS))
			{
				if (FindPS->bReadyState)
				{
					TotalReadCount++;
				}
			}
		}



		ATPSPlayerController* PC = Cast<ATPSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		//Get Server PlayerController
		if (PC && PC->LobbyWidget)
		{
			//Start 활성화
			PC->LobbyWidget->StartButton->SetIsEnabled((FindActors.Num() - 1 == TotalReadCount)); // bIsEnabled = true;
		}
	}
}
