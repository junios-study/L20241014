// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPSPlayerController.generated.h"

class ULobbyWidgetBase;

/**
 * 
 */
UCLASS()
class L20241008_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TObjectPtr<ULobbyWidgetBase> LobbyWidget;

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SendMessage(const FText& Messsage);
	bool C2S_SendMessage_Validate(const FText& Messsage);
	void C2S_SendMessage_Implementation(const FText& Message);

	UFUNCTION(Client, UnReliable)
	void S2C_AddMessage(const FText& Messsage);
	void S2C_AddMessage_Implementation(const FText& Messsage);
	
	UFUNCTION(Server, Reliable)
	void C2S_SendReadyState(bool NewState);
	void C2S_SendReadyState_Implementation(bool NewState);
};
