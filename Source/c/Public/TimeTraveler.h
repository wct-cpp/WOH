// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeTraveler.generated.h"

class AMyCharacter;

UCLASS()
class C_API ATimeTraveler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimeTraveler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneCaptureComponent2D* SceneCaptureComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "TimeTraveler")
	AMyCharacter* Character;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "TimeTraveler")
	float ZDifference=-20000.f;
};
