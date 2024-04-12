// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MySpline.generated.h"

class AMyBarrier;
class USplineComponent;

UCLASS()
class C_API AMySpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMySpline();

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USplineComponent* SplineComp;

	UPROPERTY(EditDefaultsOnly, Category = "SplineMesh")
	UStaticMesh* SplineMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "SplineMesh")
	UMaterial* SplineMaterial;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="PBoardSlot")
	TSubclassOf<AMyBarrier> BarrierClass;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CalculateSpline();

	void SpawnSplinePath(const TArray<FVector>& PathPoints);

	void GenerateGrid(const TArray<FVector>& PathPoints);

};
