// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WDialogueWidget.generated.h"

class UWDialogueSystemComponent;
/**
 * 
 */
UCLASS()
class WDIALOGUESYSTEM_API UWDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DialogueSystem")
	UWDialogueSystemComponent* DialogueSystemComp;
public:
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	bool bDisplayOptionSeparately;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	UUserWidget* OptionsWidget;


public:
	void SetDialogueSystemComp(UWDialogueSystemComponent* Comp)
	{
		DialogueSystemComp = Comp;
	}

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "DialogueSystem")
	void UpdateDialogueText(int32 DialogueIndex,const FText& Speaker, const FText& Dialogue, const TArray<FText>& OptionsText);
};
