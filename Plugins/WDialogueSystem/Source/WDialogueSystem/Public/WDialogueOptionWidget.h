// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WDialogueOptionWidget.generated.h"

class UTextBlock;
class UWDialogueSystemComponent;
/**
 * 
 */
UCLASS()
class WDIALOGUESYSTEM_API UWDialogueOptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;
protected:


public:

};
