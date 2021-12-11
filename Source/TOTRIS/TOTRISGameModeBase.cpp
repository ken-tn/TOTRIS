// Copyright Epic Games, Inc. All Rights Reserved.


#include "TOTRISGameModeBase.h"
#include <ControllerPawn.h>

void ATOTRISGameModeBase::GenerateBoard()
{
	for (int i = 0; i < BOARD_WIDTH; i++)
	{
		TArray<int> Row;
		for (int h = 0; h < BOARD_HEIGHT; h++)
		{
			Row.Add(0);
		}

		BOARD.Add(Row);
	}
}

void ATOTRISGameModeBase::RenderBoard()
{
	for (auto row : BOARD)
	{
		for (auto val : BOARD)
		{

		}
	}
}

void ATOTRISGameModeBase::GameTick()
{
	for (auto cube : CURRENTPIECE)
	{
		cube->SetActorLocation(cube->GetActorLocation() - FVector(FVector(0, 0, 200)));
		cube->y += 1;
		if (cube->y == 20) //temporary test
		{
			DrawPiece();
			return;
		}
	}
}

void ATOTRISGameModeBase::DrawPiece()
{
	Piece Cubes;
	Shape shape = tetris_shapes[FMath::RandRange(0, 5)];
	int x = BOARD_WIDTH / 2 - shape[0].Num() / 2;
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

ACube* ATOTRISGameModeBase::DrawCube(int x, int y, int colour)
{
	/*FActorSpawnParameters SpawnInfo;
	FTransform SpawnTransform = FTransform(FVector(0, x * 200, (BOARD_HEIGHT-y) * 200));
	AStaticMeshActor* NewCube = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), SpawnTransform, SpawnInfo);
	NewCube->SetMobility(EComponentMobility::Movable);
	NewCube->SetActorEnableCollision(false);

	UStaticMeshComponent* StaticMesh = NewCube->GetStaticMeshComponent();
	StaticMesh->SetStaticMesh(UCubeMesh);
	StaticMesh->SetCollisionProfileName(FName("NoCollision"), false);
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->CastShadow = false;

	StaticMesh->SetMaterial(0, );*/
	ACube* Cube = GetWorld()->SpawnActor<ACube>(ACube::StaticClass());
	Cube->Init(FVector2D(x, y), TMaterialInstances[colour], UCubeMesh);

	return Cube;
}

void ATOTRISGameModeBase::RotateClockwise()
{
	
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

	//DrawCube(0, 0, 0); // reference cube
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
