// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueInterface.h"
#include "InteractInterface.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

enum class EShowType : uint8;
class USphereComponent;
class UWDialogueSystemComponent;
class AFixedCamera;
class ATimeTraveler;
class AMyBarrier;
class UBoxComponent;
class UCameraComponent;
class USpringArmComponent;

UENUM(BlueprintType)
enum class EViewType : uint8
{
	FP UMETA(DisplayName = "FP"),
	TP UMETA(DisplayName = "TP"),
	FixedCamera UMETA(DisplayName = "FixedCamera"),
};

UCLASS()
class C_API AMyCharacter : public ACharacter , public IDialogueInterface ,public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category="Components")
	UCameraComponent* CameraComp_Third;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category="Components")
	USpringArmComponent* SpringArmComp_Third;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category="Components")
	UCameraComponent* CameraComp_First;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category="Components")
	USpringArmComponent* SpringArmComp_First;
	
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category="Components")
	UWDialogueSystemComponent* DialogueSystemComp;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category="Components")
	UBoxComponent* BoxComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void AddControllerYawInput(float Val) override;

	UFUNCTION()
	void NotifyActorBeginOverlap_BoxComp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void NotifyActorEndOverlap_BoxComp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Zoom(float AxisValue);

	void MoveForward(float Value);

	void MoveRight(float Value);

	void Grab();

	void SetViewToFixedCamera();

	void Interact();

	void DectectInteractable();

	//////////////////////////////////////////////////////////////////////////
	///IDialogueInterface
	UFUNCTION()
	void InitConversation_Implementation(AActor* InstigateActor,EShowType DialogueShowType) override;
	
	UFUNCTION()
	void ConversationClosed_Implementation() override;

	UFUNCTION()
	void SetConversableState_Implementation(bool bCanShowInteract) override;
	//////////////////////////////////////////////////////////////////////////
	///

	void InitInteractWidget();

	//////////////////////////////////////////////////////////////////////////
	///IInteractInterface
	UFUNCTION()
	void CloseInteract_Implementation() override;
	//////////////////////////////////////////////////////////////////////////
protected:
	AMyBarrier* OverlappingBarrier;

	AMyBarrier* GrabedBarrier;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "TimeTraveler")
	ATimeTraveler* TimeTraveler;

	AFixedCamera* FixedCamera;

	AActor* Conversable;

	UPROPERTY(BlueprintReadWrite)
	EViewType ViewType;

	UPROPERTY(BlueprintReadWrite)
	bool bInConversation;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Dialogue")
	EShowType DialogueShowType;

	AActor* Interactable;

	UPROPERTY(BlueprintReadOnly)
	bool bInInteract;

public:
	void SetOverlappingBarrier(AMyBarrier* Barrier)
	{
		OverlappingBarrier=Barrier;
	}

	AMyBarrier* GetOverlappingBarrier() const
	{
		return OverlappingBarrier;
	}

	void SetTimeTraveler(ATimeTraveler* Traveler)
	{
		TimeTraveler=Traveler;
	}

	UCameraComponent* GetCameraComp_First() const
	{
		return CameraComp_First;
	}

	UFUNCTION(BlueprintCallable)
	void SetInteractable(AActor* In)
	{
		Interactable=In;
	}
};
