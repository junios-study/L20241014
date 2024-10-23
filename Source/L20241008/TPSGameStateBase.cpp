// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "TPSPlayerController.h"
#include "Widget/LobbyWidgetBase.h"
#include "Kismet/GameplayStatics.h"

void ATPSGameStateBase::OnRep_PlayerCount()
{
	if (OnChangePlayerCount.IsBound())
	{
		OnChangePlayerCount.Broadcast((uint32)PlayerCount);
	}
}

void ATPSGameStateBase::OnRep_LeftTime()
{
	//무조건 자기 PC 가져온다.
	ATPSPlayerController* PC = Cast<ATPSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (IsValid(PC) && IsValid(PC->LobbyWidget))
	{
		FString Temp = FString::Printf(TEXT("%d"), LeftTime);
		PC->LobbyWidget->SetLeftTime(Temp);
	}
}

void ATPSGameStateBase::DecreaseLeftTime()
{
	LeftTime--;
	LeftTime = FMath::Clamp(LeftTime, 0, 60);
}

void ATPSGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSGameStateBase, PlayerCount);
	DOREPLIFETIME(ATPSGameStateBase, LeftTime);

}

