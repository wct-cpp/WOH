// Fill out your copyright notice in the Description page of Project Settings.


#include "WDialogueSystemComponent.h"
#include "Components/AudioComponent.h"
#include "NPC.h"
#include "WDialogueWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Engine/StreamableManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/AssetManager.h"

// Sets default values for this component's properties
UWDialogueSystemComponent::UWDialogueSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UWDialogueSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if(bUseAudio)
	{
		AudioComp = Cast<UAudioComponent>(GetOwner()->AddComponentByClass(UAudioComponent::StaticClass(),false,FTransform(),nullptr));
	}
}

// Called every frame
void UWDialogueSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWDialogueSystemComponent::OpenConversation(EShowType DialogueShowType)
{
	ShowType=DialogueShowType;
	if(ANPC* Npc = Cast<ANPC>(GetOwner()))
	{
		if (Npc->WidgetComp && ShowType==EShowType::ShowInWorld)
		{
			DialogueWidget=CreateWidget<UWDialogueWidget>(GetWorld(),DialogueWidgetClass);
			if(DialogueWidget)
			{
				DialogueWidget->SetDialogueSystemComp(this);
				Npc->WidgetComp->SetWidget(DialogueWidget);
				Npc->WidgetComp->SetVisibility(true);
			}
			if(bDisplayOptionSeparately){
				OptionsWidget=CreateWidget<UUserWidget>(GetWorld(),OptionsWidgetClass);
				if(OptionsWidget)
				{
					DialogueWidget->bDisplayOptionSeparately=bDisplayOptionSeparately;
					DialogueWidget->OptionsWidget=OptionsWidget;
					OptionsWidget->AddToViewport();
				}
			}
			ProgressDialogue();
		}
		else
		{
			if(DialogueWidgetClass)
			{GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("111111111111111111"));
				DialogueWidget=CreateWidget<UWDialogueWidget>(GetWorld(),DialogueWidgetClass);
				if(DialogueWidget)
				{GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("22222222222222222"));
					DialogueWidget->SetDialogueSystemComp(this);
					DialogueWidget->AddToViewport();
					ProgressDialogue();
				}
			}
		}
	}
}

void UWDialogueSystemComponent::CloseConversation()
{
	if(ShowType==EShowType::AddToViewport)
	{
		DialogueWidget->RemoveFromParent();
		DialogueWidget = nullptr;
		
	}
	else if(ShowType==EShowType::ShowInWorld)
	{
		Cast<ANPC>(GetOwner())->WidgetComp->SetWidget(nullptr);
		DialogueWidget = nullptr;
		
		if(bDisplayOptionSeparately){
			OptionsWidget->RemoveFromParent();
			OptionsWidget = nullptr;
		}
	}

	if(AudioComp)
	{
		AudioComp->Stop();
	}
	
	if(ANPC* Npc = Cast<ANPC>(GetOwner()))
	{
		Npc->StopAnimMontage();
	}
	
	if(GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UDialogueInterface::StaticClass()))
	{
		IDialogueInterface::Execute_ConversationClosed(GetOwner());
	}
}

void UWDialogueSystemComponent::ProgressDialogue()
{
	DialogueIndex=-1;
	Dialogue();
}

void UWDialogueSystemComponent::Dialogue_Implementation()
{
}

void UWDialogueSystemComponent::AddDialogue(const FText& Speaker, const FText& Dialogue, const TArray<FText>& OptionsText,TSoftObjectPtr<USoundBase> DialogueAudio,TSoftObjectPtr<UAnimMontage> Animation ,EDialogueState& State,int32& OptionIndex)
{
	DialogueIndex++;

	if(DialogueProgress.Find(DialogueIndex))
	{
		State=EDialogueState::PassThrough;
		OptionIndex=DialogueProgress[DialogueIndex];
		return;
	}

	DialogueWidget->UpdateDialogueText(DialogueIndex,Speaker,Dialogue,OptionsText);
	PlayAudio(DialogueAudio);
	PlayAnimation(Animation);
	State=EDialogueState::Updated;
	OptionIndex=0;
}

void UWDialogueSystemComponent::PlayAudio(TSoftObjectPtr<USoundBase> DialogueAudio)
{
	if (UKismetSystemLibrary::IsValidSoftObjectReference(DialogueAudio) && bUseAudio)
	{
		if(DialogueAudio.ToSoftObjectPath().ResolveObject())
		{
			AudioComp->SetSound(DialogueAudio.Get());
			AudioComp->Play();
		}
		else
		{
			FStreamableManager& Manager = UAssetManager::GetStreamableManager();
			// 异步加载软引用指向的对象
			TSharedPtr<FStreamableHandle> AsyncStreamHandle = Manager.RequestAsyncLoad(DialogueAudio.ToSoftObjectPath(), [this, DialogueAudio]()
			{
				AudioComp->SetSound(DialogueAudio.Get());
				AudioComp->Play();
			});

			/*回调函数另一种写法
			FStreamableDelegate Delegate;
			Delegate.BindLambda([this,DialogueAudio]()
			{
				// 在这里使用SoundBase
				AudioComp->SetSound(DialogueAudio.Get());
				AudioComp->Play();
			});
			TSharedPtr<FStreamableHandle> AsyncStreamHandle = Manager.RequestAsyncLoad(DialogueAudio.ToSoftObjectPath(),Delegate);*/
		}
	}
}

void UWDialogueSystemComponent::PlayAnimation(TSoftObjectPtr<UAnimMontage> Animation)
{
	if(UKismetSystemLibrary::IsValidSoftObjectReference(Animation))
	{
		if(Animation.ToSoftObjectPath().ResolveObject())
		{
			GetOwner()->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance()->Montage_Play(Animation.Get());
		}
		else
		{
			FStreamableManager& Manager = UAssetManager::GetStreamableManager();

			// 异步加载软引用指向的对象
			TSharedPtr<FStreamableHandle> AsyncStreamHandle = Manager.RequestAsyncLoad(Animation.ToSoftObjectPath(), [this, Animation]()
			{
				GetOwner()->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance()->Montage_Play(Animation.Get(),1.f);
			});
		}
	}
}

void UWDialogueSystemComponent::UpdateSelectedOption(int32 ProgressIndex,int32 OptionIndex)
{
	DialogueProgress.Add(ProgressIndex,OptionIndex);

	ProgressDialogue();
}

void UWDialogueSystemComponent::ClearDialogueProgress()
{
	DialogueProgress.Empty();
}

void UWDialogueSystemComponent::RemoveDialogueProgress(int32 ProgressSteps)
{
	DialogueProgress.Remove(DialogueIndex);
	for(int32 i=0;i<ProgressSteps;i++)
	{
		DialogueIndex--;
		DialogueProgress.Remove(DialogueIndex);
	}
}

void UWDialogueSystemComponent::AddDialogueProgress(int32 ProgressSteps)
{
	for(int32 i=0;i<ProgressSteps;i++)
	{
		DialogueIndex+=i;
		DialogueProgress.Add(DialogueIndex);
	}
}

void UWDialogueSystemComponent::SetDialogueTreeIndex(int32 DTI)
{
	DialogueTreeIndex=DTI;

	ClearDialogueProgress();
}

