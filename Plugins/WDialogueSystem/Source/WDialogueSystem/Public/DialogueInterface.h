// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WDialogueSystemComponent.h"
#include "DialogueInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI,BlueprintType)
class UDialogueInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WDIALOGUESYSTEM_API IDialogueInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent ,Category="DialogueSystem")
	void Interact(AActor* InstigateActor,EShowType ShowType);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="DialogueSystem")
	void ConversationClosed();
	
	UFUNCTION(BlueprintCallable,  BlueprintNativeEvent,Category="DialogueSystem")
	void SetConversableState(bool bCanShowInteract);
};
