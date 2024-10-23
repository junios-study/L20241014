// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TPSGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FChangePlayerCount, ATPSGameStateBase, OnChangePlayerCount, uint32, NewAliveCount);

/**
 * 
 */
UCLASS()
class L20241008_API ATPSGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data", ReplicatedUsing = "OnRep_PlayerCount")
	uint8 PlayerCount = 0;

	UFUNCTION()
	void OnRep_PlayerCount();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable, Category = "Data")
	FChangePlayerCount OnChangePlayerCount;
};
