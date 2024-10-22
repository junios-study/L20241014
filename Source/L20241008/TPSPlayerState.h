// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TPSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class L20241008_API ATPSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ATPSPlayerState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", ReplicatedUsing = "OnRep_ReadyState")
	uint8 bReadyState : 1;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_ReadyState();

};
