// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WDialogueSystemComponent.generated.h"


class UWDialogueWidget;

UENUM(BlueprintType)
enum class EDialogueState : uint8
{
	PassThrough UMETA(DisplayName = "PassThrough"),
	Updated UMETA(DisplayName = "Updated"),
};

UENUM(BlueprintType)
enum class EShowType : uint8
{
	ShowInWorld UMETA(DisplayName = "ShowInWorld"),
	AddToViewport UMETA(DisplayName = "AddToViewport"),
};

UCLASS( Blueprintable,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WDIALOGUESYSTEM_API UWDialogueSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWDialogueSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	TMap<int32,int32> DialogueProgress;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	int32 DialogueIndex;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	int32 DialogueTreeIndex;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Dialogue")
	TSubclassOf<UWDialogueWidget> DialogueWidgetClass;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	UWDialogueWidget* DialogueWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	FText OwnerName;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Dialogue")
	bool bUseAudio;

	UAudioComponent* AudioComp;
	
	EShowType ShowType;
	
	/*ForDisplayOptionSeparately*/
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Dialogue")
	bool bDisplayOptionSeparately;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Dialogue")
	TSubclassOf<UUserWidget> OptionsWidgetClass;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	UUserWidget* OptionsWidget;
	/*ForDisplayOptionSeparately*/

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenConversation(EShowType DialogueShowType);

	UFUNCTION(BlueprintCallable, Category = "WDialogueSystem")
	void CloseConversation();

	void ProgressDialogue();

	UFUNCTION(BlueprintNativeEvent, Category = "WDialogueSystem")
	void Dialogue();
	virtual void Dialogue_Implementation();

	UFUNCTION(BlueprintCallable, Category = "WDialogueSystem")
	void AddDialogue(const FText& Speaker, const FText& Dialogue, const TArray<FText>& OptionsText,TSoftObjectPtr<USoundBase> DialogueAudio,TSoftObjectPtr<UAnimMontage> Animation , EDialogueState& State,int32& OptionIndex);

	void PlayAudio(TSoftObjectPtr<USoundBase> DialogueAudio);

	void PlayAnimation(TSoftObjectPtr<UAnimMontage> Animation);
	
	UFUNCTION(BlueprintCallable, Category = "WDialogueSystem")
	void UpdateSelectedOption(int32 ProgressIndex,int32 OptionIndex);

	UFUNCTION(BlueprintCallable, Category = "WDialogueSystem")
	void ClearDialogueProgress();

	UFUNCTION(BlueprintCallable, Category = "WDialogueSystem")
	void RemoveDialogueProgress(int32 ProgressSteps);

	UFUNCTION(BlueprintCallable, Category = "WDialogueSystem")
	void SetDialogueTreeIndex(int32 DTI);

public:
	void SetOwnerName(const FText& InOwnerName) { OwnerName = InOwnerName; }
};
