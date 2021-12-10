// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerPawn.h"
#include <TOTRIS/TOTRISGameModeBase.h>

// Sets default values
AControllerPawn::AControllerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

#pragma region Inputs
void AControllerPawn::OnKeyDown(FKey Key)
{
	const FString KeyName = *Key.GetDisplayName().ToString().ToLower();
	//UE_LOG(LogTemp, Display, TEXT("Key: %s"), *KeyName);
	if (KeyName == "up")
	{
		ATOTRISGameModeBase* gm = Cast<ATOTRISGameModeBase>(GetWorld()->GetAuthGameMode()); // no client/server lol
		gm->RotateClockwise();

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
}
