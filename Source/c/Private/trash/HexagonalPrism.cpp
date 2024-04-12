// Fill out your copyright notice in the Description page of Project Settings.


#include "trash/HexagonalPrism.h"

// Sets default values
AHexagonalPrism::AHexagonalPrism()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent=StaticMesh;
}

// Called when the game starts or when spawned
void AHexagonalPrism::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHexagonalPrism::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

