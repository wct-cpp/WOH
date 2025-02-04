// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,BlueprintType)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class C_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent ,Category="Interact")
	void InitWidget();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent ,Category="Interact")
	void SetInteractableState(bool bCanShowInteract);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent ,Category="Interact")
	void Interact(AActor* InstigateActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent ,Category="Interact")
	void CloseInteract();
};
