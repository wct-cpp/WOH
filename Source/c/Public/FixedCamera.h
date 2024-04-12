// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FixedCamera.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class C_API AFixedCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFixedCamera();

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category="Components")
	UCameraComponent* CameraComp;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category="Components")
	USpringArmComponent* SpringArmComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float ZoomLevel;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RotateCamera(float AxisValue);
	void ZoomCamera(float AxisValue);
};
