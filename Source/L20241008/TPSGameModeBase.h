// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPSGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FChangePlayerCount, ATPSGameModeBase, OnChangePlayerCount, uint32, NewAliveCount);
/**
 * 
 */
UCLASS()
class L20241008_API ATPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options,
		const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;


	virtual void Logout(AController* Exiting) override;

	UPROPERTY(BlueprintAssignable, Category = "Data")
	FChangePlayerCount OnChangePlayerCount;

	
};
