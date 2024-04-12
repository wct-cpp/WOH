// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexWorld.generated.h"

class AHexagonalPrism;
struct Point {
	double x, y;
};
UCLASS()
class C_API AHexWorld : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AHexWorld();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "HexWorld")
	TSubclassOf<AHexagonalPrism> HexagonalPrismClass;
	
	//层数
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="HexWorld")
	int level=2;
	
	// The size of each hex
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="HexWorld")
	float HexSize;

	std::vector<Point> intersections;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Generate the world
	void GenerateWorld();

	//给定两条线段，函数内部看做直线，通过onSegment判断计算出的交点是否在线段内，得出交点
	std::pair<double, double> calculateIntersection(Point p1, Point p2, Point p3, Point p4);

	//判断点是否在线段内
	bool onSegment(Point p, Point q, Point r);

	void SpawnHex(double x,double y);
private:







	
};
