// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexagonalPrism.generated.h"

UCLASS()
class C_API AHexagonalPrism : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexagonalPrism();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
