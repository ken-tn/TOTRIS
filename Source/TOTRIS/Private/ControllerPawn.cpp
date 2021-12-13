// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <TOTRIS/TOTRISGameModeBase.h>

// Sets default values
AControllerPawn::AControllerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create our components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	//Attach our components
	StaticMeshComp->SetupAttachment(RootComponent);
	SpringArmComp->SetupAttachment(StaticMeshComp);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->SetProjectionMode(ECameraProjectionMode::Orthographic);
	CameraComp->OrthoWidth = 9000.f;

	//Move camera
	StaticMeshComp->SetWorldTransform(FTransform(FVector(-4690.000000, 730.000000, 2110.000000)));

	//Assign SpringArm class variables.
	//SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 3.0f;
}

// todo: hold drop

#pragma region Inputs
void AControllerPawn::OnKeyDown(FKey Key)
{
	const FString KeyName = *Key.GetDisplayName().ToString().ToLower();
	//UE_LOG(LogTemp, Display, TEXT("Key: %s"), *KeyName);
	ATOTRISGameModeBase* gm = Cast<ATOTRISGameModeBase>(GetWorld()->GetAuthGameMode()); // no client/server lol
	if (KeyName == "up")
	{
		gm->RotateClockwise();

		return;
	}
	else if (KeyName == "left")
	{
		gm->Move(FVector2D(-1, 0));
	}
	else if (KeyName == "right")
	{
		gm->Move(FVector2D(1, 0));
	}
	else if (KeyName == "down")
	{
		gm->Move(FVector2D(0, 1));
	}
	else if (KeyName == "escape")
	{
		gm->TogglePause();
	}
}

void AControllerPawn::OnKeyUp(FKey Key)
{
	const FString KeyName = *Key.GetDisplayName().ToString().ToLower();
	//UE_LOG(LogTemp, Display, TEXT("Key: %s"), *KeyName);
	ATOTRISGameModeBase* gm = Cast<ATOTRISGameModeBase>(GetWorld()->GetAuthGameMode()); // no client/server lol
	if (KeyName == "down")
	{
		return;
	}
}
#pragma endregion Inputs

// Called when the game starts or when spawned
void AControllerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AControllerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AControllerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("KeyDown", IE_Pressed, this, &AControllerPawn::OnKeyDown);
	PlayerInputComponent->BindAction("KeyUp", IE_Released, this, &AControllerPawn::OnKeyUp);
}
