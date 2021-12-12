// Copyright Epic Games, Inc. All Rights Reserved.


#include "TOTRISGameModeBase.h"
#include <ControllerPawn.h>

bool ATOTRISGameModeBase::CheckCollision(FVector2D PosOffset)
{
	for (ACube* Cube : CURRENTPIECE)
	{
		int offx = Cube->x + PosOffset.X;
		int offy = Cube->y + PosOffset.Y;
		if (BOARD.IsValidIndex(offx))
		{
			if (BOARD[offx].IsValidIndex(offy))
			{
				if (BOARD[offx][offy] != 0)
				{
					return true;
				}
			}
			else
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	
	return false;
}

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
	/*for (auto row : BOARD)
	{
		for (auto val : BOARD)
		{

		}
	}*/
}

bool ATOTRISGameModeBase::Move(FVector2D Offset)
{
	if (GAME_STATE != GAME_OVER && GAME_STATE != GAME_PAUSED)
	{
		if (!CheckCollision(Offset))
		{
			for (ACube* Cube : CURRENTPIECE)
			{
				Cube->x += Offset.X;
				Cube->y += Offset.Y;
				Cube->RecalculateTransform();
			}

			return true;
		}
	}

	return false;
}

void ATOTRISGameModeBase::GameTick()
{
	if (GAME_STATE == GAME_OVER || GAME_STATE == GAME_PAUSED)
	{
		return;
	}

	if (!Move(FVector2D(0, 1)))
	{
		for (ACube* Cube : CURRENTPIECE)
		{
			BOARD[Cube->x][Cube->y] = Cube->col;
		}

		DrawPiece();
		return;
	}
}

void ATOTRISGameModeBase::DrawPiece()
{
	Piece Cubes;
	Shape shape = tetris_shapes[FMath::RandRange(0, 5)];
	int x = BOARD_WIDTH / 2 - shape[0].Num() / 2;
	for (int rowIndex = 0; rowIndex < shape.Num(); rowIndex++)
	{
		TArray<int> row = shape[rowIndex];
		for (int colIndex = 0; colIndex < row.Num(); colIndex++)
		{
			int colour = row[colIndex];
			if (colour != 0) // where there is a colour
			{
				Cubes.Add(DrawCube(rowIndex + x, colIndex, colour));
			}
		}
	}

	CURRENTPIECE = Cubes;
	if (CheckCollision(FVector2D(0, 1)))
	{
		GAME_STATE = GAME_OVER;
	}
}

ACube* ATOTRISGameModeBase::DrawCube(int x, int y, int colour)
{
	ACube* Cube = GetWorld()->SpawnActor<ACube>(ACube::StaticClass());
	Cube->Init(FVector2D(x, y), TMaterialInstances[colour], UCubeMesh, colour);

	return Cube;
}

void ATOTRISGameModeBase::RotateClockwise()
{

	for (ACube* Cube : CURRENTPIECE)
	{
		Cube->RecalculateTransform();
	}

	return;
}

ATOTRISGameModeBase::ATOTRISGameModeBase()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	for (FString FMaterialReference : TMaterialReferences)
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

void ATOTRISGameModeBase::TogglePause()
{
	GAME_STATE = GAME_STATE ? GAME_RUNNING : GAME_PAUSED;
}

void ATOTRISGameModeBase::BeginPlay()
{
	GAME_STATE = GAME_RUNNING;
	GenerateBoard();
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
