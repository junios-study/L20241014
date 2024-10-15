// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidgetBase.generated.h"


class UScrollBox;
class UEditableTextBox;
class UTextBlock;
/**
 * 
 */
UCLASS()
class L20241008_API ULobbyWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	class UTextBlock* LeftTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UTextBlock* AliveCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UScrollBox* ChatScroll;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UEditableTextBox* ChatBox;

	UFUNCTION()
	void OnCommittedText(const FText& Text, ETextCommit::Type CommitMethod);

};
