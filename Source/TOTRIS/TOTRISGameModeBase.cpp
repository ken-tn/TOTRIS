// Copyright Epic Games, Inc. All Rights Reserved.


#include "TOTRISGameModeBase.h"
#include "Engine/StaticMeshActor.h"
#include <ControllerPawn.h>

void ATOTRISGameModeBase::GenerateBoard()
{
	for (int i = 0; i < BOARD_SIZE[0]; i++)
	{
		TArray<int> Row;
		for (int h = 0; h < BOARD_SIZE[1]; h++)
		{
			Row.Add(0);
		}

		BOARD.Add(Row);
	}
}

void ATOTRISGameModeBase::GameTick()
{
	for (auto cube : CURRENTPIECE)
	{
		cube->SetActorLocation(cube->GetActorLocation() - FVector(FVector(0, 0, 200)));
	}
}

void ATOTRISGameModeBase::DrawPiece()
{
	Piece Cubes;
	Shape shape = tetris_shapes[FMath::RandRange(0, 5)];
	int x = BOARD_SIZE[0] / 2 - shape[0].Num() / 2;
	int y = 0;
	for (int rowIndex = 0; rowIndex < shape.Num(); rowIndex++)
	{
		TArray<int> row = shape[rowIndex];
		for (int colIndex = 0; colIndex < row.Num(); colIndex++)
		{
			int colour = row[colIndex];
			if (colour != 0) // where there is a colour
			{
				Cubes.Add(DrawCube(rowIndex + x, colIndex + y, colour));
			}
		}
	}

	CURRENTPIECE = Cubes;
}

AActor* ATOTRISGameModeBase::DrawCube(int x, int y, int colour)
{
	FActorSpawnParameters SpawnInfo;
	FTransform SpawnTransform = FTransform(FVector(0, x * 200, (BOARD_SIZE[1]-y) * 200));
	AStaticMeshActor* NewCube = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), SpawnTransform, SpawnInfo);
	NewCube->SetMobility(EComponentMobility::Movable);
	NewCube->SetActorEnableCollision(false);

	UStaticMeshComponent* StaticMesh = NewCube->GetStaticMeshComponent();
	StaticMesh->SetStaticMesh(UCubeMesh);
	StaticMesh->SetCollisionProfileName(FName("NoCollision"), false);
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->CastShadow = false;

	StaticMesh->SetMaterial(0, TMaterialInstances[colour]);

	return NewCube;
}

void ATOTRISGameModeBase::RotateClockwise()
{
	UE_LOG(LogTemp, Warning, TEXT("hehe"));
	return;
}

ATOTRISGameModeBase::ATOTRISGameModeBase()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	for (auto FMaterialReference : TMaterialReferences)
	{
		const TCHAR* text = *FMaterialReference;
		ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant>MaterialAsset(text);

		if (MaterialAsset.Object != nullptr)
		{
			TMaterialInstances.Add(MaterialAsset.Object);
		}
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh>CubeMesh(TEXT("StaticMesh'/Game/Meshes/kyoob.kyoob'"));
	if (CubeMesh.Object != nullptr)
	{
		UCubeMesh = CubeMesh.Object;
	}
}

void ATOTRISGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	FActorSpawnParameters SpawnInfo;
	AControllerPawn* Pawn = GetWorld()->SpawnActor<AControllerPawn>(FVector(), FRotator(), SpawnInfo);
	NewPlayer->Possess(Pawn);
}

void ATOTRISGameModeBase::BeginPlay()
{
	UE_LOG(LogTemp, Display, TEXT("Board Generated"));
	GenerateBoard();

	DrawCube(0, 0, 0); // reference cube
	DrawPiece();
}

// Called every frame
void ATOTRISGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	frame++;
	if (frame >= FRAMES_PER_TICK)
	{
		frame = 0;
		GameTick();
	}
}
