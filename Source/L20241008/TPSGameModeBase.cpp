// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameModeBase.h"
#include "TPSGameStateBase.h"

void ATPSGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	UE_LOG(LogTemp, Warning, TEXT("PreLogin()"));
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	//ErrorMessage = "Get Out";
}

APlayerController* ATPSGameModeBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	//ErrorMessage = "Get Out";

	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
}

void ATPSGameModeBase::PostLogin(APlayerController* NewPlayer)
{

	ATPSGameStateBase* GS = GetGameState<ATPSGameStateBase>();
	if (IsValid(GS))
	{
		GS->PlayerCount = 0;
		for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
		{
			//살아있는 폰 갯수 세기
			GS->PlayerCount++;
			GS->OnRep_PlayerCount();
		}

		UE_LOG(LogTemp, Warning, TEXT("Connect Count %d"), GS->PlayerCount);
	}


	Super::PostLogin(NewPlayer);
}

void ATPSGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}
