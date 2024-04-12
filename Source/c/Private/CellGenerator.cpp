// Fill out your copyright notice in the Description page of Project Settings.


#include "CellGenerator.h"

// Sets default values
ACellGenerator::ACellGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACellGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACellGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ACellGenerator::GetTerrainHeight(const FVector2D Location,const float Scale,const float Amplitude)
{
	return FMath::PerlinNoise2D(Location/Scale + FVector2D(0.1f,0.1f))*Amplitude;
}