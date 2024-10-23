// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameModeBase.h"
#include "TPSGameStateBase.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "L20241008.h"

void ATPSGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	UE_LOG(LogTemp, Warning, TEXT("PreLogin()"));
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	//ErrorMessage = "Get Out";
}

void ATPSGameModeBase::ProcessLeftTime()
{
	UE_LOG(LogStudy, Warning, TEXT("ProcessLeftTime"));
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
	ATPSGameStateBase* GS = GetGameState<ATPSGameStateBase>();
	if (IsValid(GS))
	{
		GS->PlayerCount = 0;
		for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
		{
			//살아있는 폰 갯수 세기
			GS->PlayerCount++;
		}

		GS->PlayerCount--;
		GS->OnRep_PlayerCount();

		UE_LOG(LogTemp, Warning, TEXT("Connect Count %d"), GS->PlayerCount);
	}

	Super::Logout(Exiting);
}

void ATPSGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//GetWorld()->GetTimerManager().SetTimer(
	//	LeftTimeHandler,
	//	this,
	//	&ATPSGameModeBase::ProcessLeftTime,
	//	1.0f,
	//	true
	//);
	GetWorld()->GetTimerManager().SetTimer(
		LeftTimeHandler,
		FTimerDelegate::CreateLambda([&]() {
			ATPSGameStateBase* GS = GetGameState<ATPSGameStateBase>();
			if (IsValid(GS))
			{
				GS->DecreaseLeftTime();
				GS->OnRep_LeftTime();
			}
		}),
		1.0f,
		true
	);

}

void ATPSGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(LeftTimeHandler);

	Super::EndPlay(EndPlayReason);
}
