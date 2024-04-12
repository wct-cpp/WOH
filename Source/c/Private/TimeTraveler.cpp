// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeTraveler.h"
#include "Components/SceneCaptureComponent2D.h"
#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATimeTraveler::ATimeTraveler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	
	SceneCaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComp"));
	SceneCaptureComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATimeTraveler::BeginPlay()
{
	Super::BeginPlay();

	if(AMyCharacter* C = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)))
	{
		Character = C;
		Character->SetTimeTraveler(this);
	}
}

// Called every frame
void ATimeTraveler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Character)
	{
		FVector CharacterLocation = Character->GetActorLocation();
		FTransform T = FTransform(Character->GetCameraComp_First()->GetComponentRotation(),
									FVector(CharacterLocation.X,CharacterLocation.Y,CharacterLocation.Z+ZDifference),
									Character->GetActorScale());
		SetActorTransform(T);

		SceneCaptureComp->SetRelativeTransform(Character->GetCameraComp_First()->GetRelativeTransform());
	}
}

