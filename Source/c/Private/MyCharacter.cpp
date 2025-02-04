// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/SphereComponent.h"
#include "DialogueInterface.h"
#include "DrawDebugHelpers.h"
#include "FixedCamera.h"
#include "InteractInterface.h"
#include "MyBarrier.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "WDialogueSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 关闭“使用控制器旋转Yaw”
	bUseControllerRotationYaw = false;

	// 获取“角色移动”组件，然后开启“将旋转朝向运动”
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	SpringArmComp_Third = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp_Third->bUsePawnControlRotation = true;
	SpringArmComp_Third->SetupAttachment(RootComponent);
	
	CameraComp_Third = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp_Third->bUsePawnControlRotation = true;
	CameraComp_Third->SetupAttachment(SpringArmComp_Third);

	SpringArmComp_First = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp_First"));
	SpringArmComp_First->bUsePawnControlRotation = true;
	SpringArmComp_First->SetupAttachment(RootComponent);

	CameraComp_First = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp_First"));
	CameraComp_First->bUsePawnControlRotation = true;
	CameraComp_First->SetupAttachment(SpringArmComp_First);

	DialogueSystemComp = CreateDefaultSubobject<UWDialogueSystemComponent>(TEXT("DialogueSystemComp"));

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);

	OverlappingBarrier = nullptr;
	GrabedBarrier = nullptr;

	ViewType = EViewType::TP;

	bInConversation=false;
	bInInteract=false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFixedCamera::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		FixedCamera = Cast<AFixedCamera>(Actor);
	}

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::NotifyActorBeginOverlap_BoxComp);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::NotifyActorEndOverlap_BoxComp);

	InitInteractWidget();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DectectInteractable();
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMyCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Zoom", this, &AMyCharacter::Zoom);

	PlayerInputComponent->BindAction("Grab", IE_Pressed, this, &AMyCharacter::Grab);

	PlayerInputComponent->BindAction("SetViewToFixedCamera", IE_Pressed, this, &AMyCharacter::SetViewToFixedCamera);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMyCharacter::Interact);
}

void AMyCharacter::AddControllerYawInput(float Val)
{
	Super::AddControllerYawInput(Val);

	if (FixedCamera)//使用固定相机控制视角
	{
		FixedCamera->RotateCamera(Val);
	}
}

void AMyCharacter::NotifyActorBeginOverlap_BoxComp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AMyCharacter::NotifyActorEndOverlap_BoxComp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AMyCharacter::Zoom(float AxisValue)
{
	if(bInConversation)
	{
		return;
	}
	if (FixedCamera && ViewType==EViewType::FixedCamera)//使用固定相机控制视角
	{
		FixedCamera->ZoomCamera(AxisValue);
	}
	else
	{
		// AxisValue is the input from the mouse wheel
        float ZoomSpeed = 250.0f; // Adjust this value to your liking
        float NewFOV = CameraComp_Third->FieldOfView - (AxisValue * ZoomSpeed * GetWorld()->GetDeltaSeconds());
        // Clamp the FOV to reasonable values
        NewFOV = FMath::Clamp(NewFOV, 30.0f, 120.0f);
    
        CameraComp_Third->SetFieldOfView(NewFOV);
	}
}

void AMyCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	// 转向只关注水平Yaw方向，因此置0防止影响
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;
	// 获取相机（鼠标控制器）的朝向，并朝这个方向移动
	AddMovementInput(ControlRot.Vector(), Value);
}

void AMyCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;
	// 获取相机（鼠标控制器）的朝向，转向右侧，并朝这个方向移动；传入的Y表示右侧
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, Value);
}

void AMyCharacter::Grab()
{
	if (OverlappingBarrier && !GrabedBarrier)
	{
		GrabedBarrier = OverlappingBarrier;
		GrabedBarrier->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		GrabedBarrier->SetActorEnableCollision(false); // Ignore collision
		
	}
	else if (GrabedBarrier)//drop
	{
		GrabedBarrier->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		GrabedBarrier->SetActorEnableCollision(true); // Enable collision

		// Get the forward direction of the character
		FVector ForwardDirection = GetActorForwardVector();

		// Calculate the offset (distance in front of the character)
		float Distance = 70.0f; // Change this to the desired distance
		FVector Offset = ForwardDirection * Distance;

		// Calculate the new location
		FVector NewLocation = GetActorLocation() + Offset;

		// Perform a raycast from the new location downwards
		FHitResult HitResult;
		FVector Start = NewLocation;
		FVector End = Start - FVector(0, 0, 1000); // 1000 is just a large number to ensure the raycast hits the ground
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GrabedBarrier);
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
		{
			// If the raycast hit something, set the new location to the hit location
			NewLocation = HitResult.Location;
		}
		NewLocation.Z+=50.0f;
		// Set the new location of the barrier
		GrabedBarrier->SetActorLocation(NewLocation);

		GrabedBarrier = nullptr;
	}
}

void AMyCharacter::SetViewToFixedCamera()
{
	if (FixedCamera)
	{
		ViewType = EViewType::FixedCamera;
		
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			PlayerController->SetViewTargetWithBlend(FixedCamera, 0.5f);
		}
	}
}

void AMyCharacter::Interact()
{
	if(Conversable && Conversable->GetClass()->ImplementsInterface(UDialogueInterface::StaticClass()) && !bInConversation)
	{
		IDialogueInterface::Execute_InitConversation(Conversable,this,DialogueShowType);
		bInConversation=true;
	}
	if(Interactable && Interactable->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()) && !bInInteract)
	{
		bInInteract=true;
		IInteractInterface::Execute_SetInteractableState(Interactable,false);
		IInteractInterface::Execute_Interact(Interactable,this);
		
	}
}

void AMyCharacter::DectectInteractable()
{
	/*FVector StartLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector CameraForwardVector;
	if(ViewType==EViewType::TP)
	{
		CameraForwardVector = CameraComp_Third->GetForwardVector();
	}
	else
	{
		CameraForwardVector = CameraComp_First->GetForwardVector();
	}
	
	float TraceDistance = 200.0f; 
	FVector EndLocation = StartLocation + (CameraForwardVector * TraceDistance);

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this); 
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);*/
	




	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		// 获取视口的大小
		int32 ViewportSizeX, ViewportSizeY;
		PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

		// 计算屏幕中心的坐标
		FVector2D ScreenCenter(ViewportSizeX / 2, ViewportSizeY / 2);

		// 将屏幕中心的坐标转换为世界空间的射线
		FVector WorldLocation, WorldDirection;
		PlayerController->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldLocation, WorldDirection);

		// 进行射线检测
		FHitResult HitResult;
		FVector StartLocation = WorldLocation;
		FVector EndLocation = StartLocation + (WorldDirection * 10000); // 10000是射线的长度，你可以根据需要进行修改
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);
		// 绘制调试线条
		FColor LineColor = bHit ? FColor::Green : FColor::Red; // 如果射线检测到了物体，那么线条为绿色，否则为红色
		float LineLifeTime = 0.5f; // 线条存在的时间
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, false, LineLifeTime);
		
		if (bHit)
		{
			AActor* HitActor = HitResult.GetActor();
		
			if (HitActor->GetClass()->ImplementsInterface(UDialogueInterface::StaticClass()))
			{
				if(HitActor==Conversable)
				{
					return;
				}
				Conversable=HitActor;
				IDialogueInterface::Execute_SetConversableState(Conversable, true);
			}
			else
			{
				if(Conversable)
				{
					IDialogueInterface::Execute_SetConversableState(Conversable, false);
					Conversable=nullptr;
				}
			}
		
			if (HitActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			{
				if(HitActor==Interactable)
				{
					return;
				}
				if(Interactable)
				{
					IInteractInterface::Execute_SetInteractableState(Interactable,false);
				}
			
				Interactable=HitActor;
				IInteractInterface::Execute_SetInteractableState(Interactable,true);
			}
			else
			{
				if(Interactable)
				{
					IInteractInterface::Execute_SetInteractableState(Interactable,false);
					Interactable=nullptr;
				}
			}
		}
		else
		{
			if(Conversable)
			{
				IDialogueInterface::Execute_SetConversableState(Conversable, false);
				Conversable=nullptr;
			}
			if(Interactable)
			{
				IInteractInterface::Execute_SetInteractableState(Interactable,false);
				Interactable=nullptr;
			}
		}
	}
}

void AMyCharacter::InitConversation_Implementation(AActor* InstigateActor,EShowType DShowType)
{
	IDialogueInterface::InitConversation_Implementation(InstigateActor,DShowType);
}

void AMyCharacter::ConversationClosed_Implementation()
{
	IDialogueInterface::ConversationClosed_Implementation();

	bInConversation=false;

	bInInteract=false;
}

void AMyCharacter::SetConversableState_Implementation(bool bCanShowInteract)
{
	IDialogueInterface::SetConversableState_Implementation(bCanShowInteract);
}

void AMyCharacter::InitInteractWidget()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UInteractInterface::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		IInteractInterface::Execute_InitWidget(Actor);
	}
}

void AMyCharacter::CloseInteract_Implementation()
{
	IInteractInterface::CloseInteract_Implementation();

	if(Interactable)
	{
		IInteractInterface::Execute_SetInteractableState(Interactable,true);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CloseInteract"));
	bInInteract=false;
}

