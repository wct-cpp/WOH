// Fill out your copyright notice in the Description page of Project Settings.
#include "MySpline.h"

#include "MyBarrier.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMySpline::AMySpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
	
	
}

// Called when the game starts or when spawned
void AMySpline::BeginPlay()
{
	Super::BeginPlay();
	
	CalculateSpline();
}

// Called every frame
void AMySpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMySpline::CalculateSpline()
{
	TArray<FVector> PathPoints;

	for(int i=0;i<100;i++)
	{
		float time1 = i*10;
		float time2 = (i+1)*10;
		
		FVector	Point1 = SplineComp->GetComponentLocation()+FVector(time1,0,0);
		FVector	Point2 = SplineComp->GetComponentLocation()+FVector(time2,0,0);

		PathPoints.Add(Point1);

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		ActorsToIgnore.Add(GetOwner());
			
		FHitResult HitResult_T;
			
		bool LineTrace = UKismetSystemLibrary::LineTraceSingle(GetWorld(),Point1,Point2,TraceTypeQuery1,false,
			ActorsToIgnore,EDrawDebugTrace::ForOneFrame,HitResult_T,true);
		if(LineTrace)
		{
			break;
		}
	}
	SpawnSplinePath(PathPoints);
	GenerateGrid(PathPoints);
	PathPoints.Empty();
}

void AMySpline::SpawnSplinePath(const TArray<FVector>& PathPoints)
{
	SplineComp->ClearSplinePoints();//先清除之前的点

	TArray<USplineMeshComponent*> SplineMeshes;//存储所有的SplineMeshComponent
	GetComponents<USplineMeshComponent>(SplineMeshes);//获取所有的SplineMeshComponent
	for (int i=0;i<SplineMeshes.Num();i++) {
		SplineMeshes[i]->DestroyComponent();//销毁所有的SplineMeshComponent
	}

	for (int i=0;i<PathPoints.Num();i++)
	{
		//UE_LOG(LogTemp,Log,TEXT("PathPoints[%d]=%s"),i,*PathPoints[i].ToString());
		SplineComp->AddSplinePointAtIndex(PathPoints[i],i,ESplineCoordinateSpace::World,true);//添加点
	}

	//PathPoints.Empty();//清空样条点数组

	for(int i=0;i<SplineComp->GetNumberOfSplinePoints()-2;i++)
	{
		if (i%2==0)//每隔一个点创建一个SplineMeshComponent
		{
			USplineMeshComponent* SplineMeshComp=NewObject<USplineMeshComponent>(this);//创建一个SplineMeshComponent
            
			SplineMeshComp->SetMobility(EComponentMobility::Movable);//设置移动性
            
			SplineMeshComp->SetStaticMesh(SplineMesh);//设置样条线静态网格
			SplineMeshComp->SetMaterial(0,SplineMaterial);//设置材质
			SplineMeshComp->SetStartAndEnd(SplineComp->GetLocationAtSplinePoint(i,ESplineCoordinateSpace::World),
				SplineComp->GetTangentAtSplinePoint(i,ESplineCoordinateSpace::World),
				SplineComp->GetLocationAtSplinePoint(i+1,ESplineCoordinateSpace::World),
				SplineComp->GetTangentAtSplinePoint(i+1,ESplineCoordinateSpace::World),true);//设置起点和终点
			SplineMeshComp->SetForwardAxis(ESplineMeshAxis::X);//设置前向轴
			SplineMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);//设置碰撞
			SplineMeshComp->SetStartScale(FVector2D(0.3f,0.3f));//设置起始缩放
			SplineMeshComp->SetEndScale(FVector2D(0.1f,0.1f));//设置结束缩放
			SplineMeshComp->SetVisibility(true);//设置可见性
            
			SplineMeshComp->RegisterComponent();//注册组件 这个函数才能把组件添加到Actor中
		}
	}
}

void AMySpline::GenerateGrid(const TArray<FVector>& PathPoints)
{
	// Calculate the distance between the first and last points
	float TotalDistance = FVector::Dist(PathPoints[0], PathPoints.Last());

	// Define the size of the grid
	int32 GridSize = 10;

	// Calculate the spacing between each instance
	float Spacing = TotalDistance / GridSize; 

	// Generate the instances
	for (int32 i = 0; i < GridSize; ++i)
	{
		// Calculate the location for this instance
		FVector Direction = (PathPoints.Last() - PathPoints[0]).GetSafeNormal();
		FVector Location = PathPoints[0] + i * Spacing * Direction;

		if(BarrierClass)
		{
			UChildActorComponent* Child = NewObject<UChildActorComponent>(this, UChildActorComponent::StaticClass());
			if (Child) {
				Child->bEditableWhenInherited = true;
				Child->RegisterComponent();
				Child->SetChildActorClass(BarrierClass);
				Child->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
				Child->SetRelativeLocation(Location+FVector(0,0,50));
				Child->CreateChildActor();
			}
		}
	}
}

