// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameStateBase.h"
#include "Net/UnrealNetwork.h"

void ATPSGameStateBase::OnRep_PlayerCount()
{
	OnChangePlayerCount.Broadcast((uint32)PlayerCount);
}

void ATPSGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSGameStateBase, PlayerCount);
}
