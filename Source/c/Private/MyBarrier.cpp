// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBarrier.h"

#include "MyCharacter.h"
#include "Components/BoxComponent.h"
// Sets default values
AMyBarrier::AMyBarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent=CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMyBarrier::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&AMyBarrier::NotifyActorBeginOverlap_BoxComp);
	BoxComp->OnComponentEndOverlap.AddDynamic(this,&AMyBarrier::NotifyActorEndOverlap_BoxComp);
}

void AMyBarrier::NotifyActorBeginOverlap_BoxComp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AMyCharacter* C=Cast<AMyCharacter>(OtherActor))
	{
		Outline(true);
		C->SetOverlappingBarrier(this);
	}
}

void AMyBarrier::NotifyActorEndOverlap_BoxComp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(AMyCharacter* C=Cast<AMyCharacter>(OtherActor))
	{
		Outline(false);
		if (C->GetOverlappingBarrier() == this)
		{
			C->SetOverlappingBarrier(nullptr);
		}
	}
}

void AMyBarrier::Outline(bool bIsOutline)
{
	if(bIsOutline)
	{
		StaticMesh->SetRenderCustomDepth(true);
	}
	else
	{
		StaticMesh->SetRenderCustomDepth(false);
	}
}

// Called every frame
void AMyBarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

