// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelWorld.h"

// Sets default values
AVoxelWorld::AVoxelWorld()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVoxelWorld::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVoxelWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AVoxelWorld::GetTerrainHeight(const FVector2D Location,const float Scale,const float Amplitude)
{
	return FMath::PerlinNoise2D(Location/Scale + FVector2D(0.1f,0.1f))*Amplitude;
}

