// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Components/WidgetComponent.h"
#include "WDialogueSystemComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	TextRender->SetVisibility(false);
	TextRender->SetupAttachment(RootComponent);

	WidgetComp=CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetVisibility(false);
	WidgetComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
	if(DialogueSystemCompClass)
	{
		DialogueSystemComp=NewObject<UWDialogueSystemComponent>(this,DialogueSystemCompClass);
		DialogueSystemComp->SetOwnerName(NickName);
		DialogueSystemComp->RegisterComponent();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DialogueSystemCompClass is nullptr"));
	}
	
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TextRender->SetText(NickName);
}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPC::InitConversation_Implementation(AActor* InstigateActor,EShowType DShowType)
{
	IDialogueInterface::InitConversation_Implementation(InstigateActor,DShowType);

	Instigator=InstigateActor;
	
	DialogueSystemComp->OpenConversation(DShowType);
}

void ANPC::ConversationClosed_Implementation()
{
	IDialogueInterface::ConversationClosed_Implementation();

	if(Instigator)
	{
		IDialogueInterface::Execute_ConversationClosed(Instigator);
	}
}

void ANPC::SetConversableState_Implementation(bool bCanShowInteract)
{
	IDialogueInterface::SetConversableState_Implementation(bCanShowInteract);
	
	TextRender->SetVisibility(bCanShowInteract);
}

