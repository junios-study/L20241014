// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerState.h"
#include "Net/UnrealNetwork.h"

ATPSPlayerState::ATPSPlayerState()
{
	bReadyState = false;
}

void ATPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSPlayerState, bReadyState);
//	DOREPLIFETIME_CONDITION()
}

void ATPSPlayerState::OnRep_ReadyState()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName());
}

