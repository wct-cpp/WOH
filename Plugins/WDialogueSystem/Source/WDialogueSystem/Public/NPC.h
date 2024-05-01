// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueInterface.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "NPC.generated.h"

enum class EShowType : uint8;
class UWidgetComponent;
class UWDialogueSystemComponent;
class UTextRenderComponent;

UCLASS()
class WDIALOGUESYSTEM_API ANPC : public ACharacter ,public IDialogueInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWDialogueSystemComponent> DialogueSystemCompClass;

	UPROPERTY(BlueprintReadOnly)
	UWDialogueSystemComponent* DialogueSystemComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTextRenderComponent* TextRender;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* WidgetComp;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	  
	AActor* Instigator;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = "NPC")
	FText NickName=FText::FromString("NPC");

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//////////////////////////////////////////////////////////////////////////
	///IDialogueInterface
	UFUNCTION()
	void InitConversation_Implementation(AActor* InstigateActor,EShowType DialogueShowType) override;
	
	UFUNCTION()
	void ConversationClosed_Implementation() override;

	UFUNCTION()
	void SetConversableState_Implementation(bool bCanShowInteract) override;
	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintImplementableEvent, Category = "NPC")
	void AfterConversationClosed(AActor* InstigateActor);

public:
	FText GetNickName() const { return NickName; }
};
