// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"

// Sets default values
ACell::ACell()
{
	PrimaryActorTick.bCanEverTick = false;
	
	//RootComponent=CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void ACell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

