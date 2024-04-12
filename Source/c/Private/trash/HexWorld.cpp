// Fill out your copyright notice in the Description page of Project Settings.


#include "trash/HexWorld.h"

#include <limits>

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "trash/HexagonalPrism.h"

// Sets default values
AHexWorld::AHexWorld()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the hex size
	HexSize = 200.0f;//半径
}

// Called when the game starts or when spawned
void AHexWorld::BeginPlay()
{
	Super::BeginPlay();

	// Generate the world
	GenerateWorld();
}

// Called every frame
void AHexWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHexWorld::GenerateWorld()
{
	// 第一层只有一个
	if(HexagonalPrismClass)
	{
		UChildActorComponent* Child = NewObject<UChildActorComponent>(this, UChildActorComponent::StaticClass());
		if (Child) {
			Child->bEditableWhenInherited = true;
			Child->RegisterComponent();
			Child->SetChildActorClass(HexagonalPrismClass);
			Child->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			Child->CreateChildActor();
		}
	}
	
	for (int i = 1; i < level; ++i)
	{
		int n=6*i;//每层的个数
		double dis = (sqrt(3)/2)*HexSize*i;//每层的距离
		
		for (int j =0;j<n;++j)
		{
			// Calculate the location for this hex
			//把每一层的六边形中心连起来，是一个大的六边形，把这个大六边形每条线段看成一个函数（每层6个），知道了从原点出发到每层小六边形的中心的角度，这条射线也看成函数，与大六边形的每条线段的交点就是每层小六边形的中心坐标
			double angleInRadians = FMath::DegreesToRadians(90-j*360/n);
			
			Point p1 = {0, dis};
			Point p2 = {dis*FMath::Cos(FMath::DegreesToRadians(30)), dis*FMath::Sin(FMath::DegreesToRadians(30))};
			Point p3 = {dis*FMath::Cos(FMath::DegreesToRadians(30)), -dis*FMath::Sin(FMath::DegreesToRadians(30))};
			Point p4 = {0, -dis};
			Point p5 = {-dis*FMath::Cos(FMath::DegreesToRadians(30)), -dis*FMath::Sin(FMath::DegreesToRadians(30))};
			Point p6 = {-dis*FMath::Cos(FMath::DegreesToRadians(30)), dis*FMath::Sin(FMath::DegreesToRadians(30))};

			//原点出发到小六边形的中心的角度的线段，与半径为dis的圆接壤，对于在与小六边形的边相交的线段相比大一点点
			Point p7= {0, 0};
			Point p8={dis*FMath::Cos(angleInRadians),dis*FMath::Sin(angleInRadians)};
			
			calculateIntersection(p1,p2,p7,p8);
			calculateIntersection(p2,p3,p7,p8);
			calculateIntersection(p3,p4,p7,p8);
			calculateIntersection(p4,p5,p7,p8);
			calculateIntersection(p5,p6,p7,p8);
			calculateIntersection(p6,p1,p7,p8);
		}
	}
}

bool AHexWorld::onSegment(Point p, Point q, Point r)
{
	double epsilon = 1.0f; // Define epsilon 由于浮点数的精度问题导致结果不准确，引入一个小的误差范围（称为epsilon）来处理这个问题。
	if (q.x <= std::max(p.x, r.x) + epsilon && q.x >= std::min(p.x, r.x) - epsilon &&
		q.y <= std::max(p.y, r.y) + epsilon && q.y >= std::min(p.y, r.y) - epsilon)
    	return true;
    return false;
}

void AHexWorld::SpawnHex(double x,double y)
{
	if(HexagonalPrismClass)
	{
		UChildActorComponent* Child = NewObject<UChildActorComponent>(this, UChildActorComponent::StaticClass());
		if (Child) {
			Child->bEditableWhenInherited = true;
			Child->RegisterComponent();
			Child->SetChildActorClass(HexagonalPrismClass);
			Child->AddRelativeLocation(FVector(x,y,0));
			//Child->SetRelativeLocation(Location);
			Child->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			Child->CreateChildActor();
			GEngine->AddOnScreenDebugMessage(-1, 53.f, FColor::Red, TEXT("SpawnHex"));
		}
	}
}

std::pair<double, double> AHexWorld::calculateIntersection(Point p1, Point p2, Point p3, Point p4)
{
	double a1 = p2.y - p1.y;
	double b1 = p1.x - p2.x;
	double c1 = a1 * p1.x + b1 * p1.y;

	double a2 = p4.y - p3.y;
	double b2 = p3.x - p4.x;
	double c2 = a2 * p3.x + b2 * p3.y;

	double determinant = a1 * b2 - a2 * b1;

	if (determinant == 0) { 
		// The lines are parallel
		return std::make_pair(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN());
	} else {
		double x = (b2 * c1 - b1 * c2) / determinant;
		double y = (a1 * c2 - a2 * c1) / determinant;
		Point intersection = {x, y};

		/*// Check if the intersection point already exists in the vector
		for (const auto& point : intersections) {
			if (abs(point.x - intersection.x) < 5.0f && abs(point.y - intersection.y) < 5.0f) {
				// The intersection point already exists, so return
				return std::make_pair(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN());
			}
		}*/
		
		// The intersection point does not exist in the vector, so add it
		intersections.push_back(intersection);
		
		if (onSegment(p1, intersection, p2) && onSegment(p3, intersection, p4)) {
			
			SpawnHex(x,y);
			return std::make_pair(x, y);
		} else {
			// The intersection is not on the line segments
			return std::make_pair(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN());
		}
	}
}

