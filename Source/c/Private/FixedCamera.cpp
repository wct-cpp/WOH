// Fill out your copyright notice in the Description page of Project Settings.


#include "FixedCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AFixedCamera::AFixedCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// Initialize the ZoomLevel
	ZoomLevel = 1000.0f;
	SpringArmComp->TargetArmLength = ZoomLevel;
}

// Called when the game starts or when spawned
void AFixedCamera::BeginPlay()
{
	Super::BeginPlay();
	
	SpringArmComp->SetWorldRotation(FRotator(-40.0f, 0.0f, 0.0f));
}

// Called every frame
void AFixedCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SpringArmComp->TargetArmLength = ZoomLevel;
}

void AFixedCamera::RotateCamera(float AxisValue)
{
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += AxisValue;
	SpringArmComp->SetWorldRotation(NewRotation);
}

void AFixedCamera::ZoomCamera(float AxisValue)
{
	ZoomLevel =FMath::Clamp(ZoomLevel+AxisValue * 100.0f,100.0f,2000.0f);
}

