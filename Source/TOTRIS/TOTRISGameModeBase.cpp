// Copyright Epic Games, Inc. All Rights Reserved.


#include "TOTRISGameModeBase.h"
#include <ControllerPawn.h>

bool ATOTRISGameModeBase::IsCollision(FVector2D Pos)
{
	int X = Pos.X;
	int Y = Pos.Y;
	if (BOARD.IsValidIndex(X))
	{
		if (BOARD[X].IsValidIndex(Y))
		{
			if (BOARD[X][Y] != 0)
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

	return false;
}

bool ATOTRISGameModeBase::IsPieceColliding(FVector2D Offset)
{
	for (ACube* Cube : CURRENTPIECE)
	{
		int offx = Cube->x + Offset.X;
		int offy = Cube->y + Offset.Y;
		
		if (IsCollision(FVector2D(offx, offy)))
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
		TArray<ACube*> CubeRow;
		for (int h = 0; h < BOARD_HEIGHT; h++)
		{
			Row.Add(0);
		}

		BOARD.Add(Row);
		BOARDCUBES.Add(CubeRow);
	}
}

bool ATOTRISGameModeBase::Move(FVector2D Offset)
{
	if (GAME_STATE != GAME_OVER && GAME_STATE != GAME_PAUSED)
	{
		if (!IsPieceColliding(Offset))
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

		/*
		for (int row : BOARD)
		{
			for (int val : row)
			{
				if (val == 0)
				{
					continue;
				}

				for (int val : row)
				{
					val = 0;

					//BOARDCUBES.
				}
			}
		}
		*/

		DrawPiece();
		return;
	}
}

void ATOTRISGameModeBase::DrawPiece(const int& shapeIndex)
{
	Shape shape = tetris_shapes[shapeIndex];
	Piece Cubes;
	int x = BOARD_WIDTH / 2 - shape[0].Num() / 2;
	for (int rowIndex = 0; rowIndex < shape.Num(); rowIndex++)
	{
		TArray<int> row = shape[rowIndex];
		for (int colIndex = 0; colIndex < row.Num(); colIndex++)
		{
			if (row[colIndex] != 0) // where there is a colour
			{
				Cubes.Add(DrawCube(colIndex + x, rowIndex, shapeIndex));
			}
		}
	}

	CURRENTPIECE = Cubes;
	if (IsPieceColliding(FVector2D(0, 1)))
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

Shape ATOTRISGameModeBase::RotateMatrix(Shape shape)
{
	//Transpose
	//Reverse each row
	Shape rotatedMatrix;
	for (int x = 0; x < shape[0].Num(); x++)
	{
		TArray<int> row;
		for (int y = shape.Num()-1; y > -1; y--)
		{
			row.Add(shape[y][x]);
		}
		rotatedMatrix.Add(row);
	}

	/*
	anti-clockwise pain
	for (int x = shape[0].Num() - 1; x > -1; x--)
	{
		TArray<int> l;
		for (int y = 0; y < shape.Num(); y++)
		{
			l.Add(shape[y][x]);
		}
		rotatedMatrix.Add(l);
	}*/

	return rotatedMatrix;
}

void ATOTRISGameModeBase::RotateClockwise()
{
	int pivotIndex = 0;
	ACube* pivot = CURRENTPIECE[pivotIndex];
	int rotations = (pivot->rotations + 1) % 4;
	int colour = pivot->col;
	Shape shape = tetris_shapes[colour];
	for (int i = 0; i < rotations; i++) {
		shape = RotateMatrix(shape);
	}

	FVector2D offset = NESOffsets[colour][rotations];
	int offx = pivot->x + offset.X;
	int offy = pivot->y + offset.Y;

	TMap<FVector2D, int> NewCubePositions;
	for (int rowIndex = 0; rowIndex < shape.Num(); rowIndex++)
	{
		TArray<int> row = shape[rowIndex];
		for (int colIndex = 0; colIndex < row.Num(); colIndex++)
		{
			if (row[colIndex] != 0)
			{
				FVector2D newPos = FVector2D(colIndex + offx, rowIndex + offy);
				if (IsCollision(newPos))
				{
					return;
				}
				NewCubePositions.Add(newPos, colour);
			}
		}
	}

	for (ACube* Cube : CURRENTPIECE)
	{
		Cube->Destroy();
	}

	Piece Cubes;
	for (TPair<FVector2D, int> CubeData : NewCubePositions)
	{
		Cubes.Add(DrawCube(CubeData.Key.X, CubeData.Key.Y, CubeData.Value));
	}

	CURRENTPIECE = Cubes;
	CURRENTPIECE[pivotIndex]->rotations = rotations;

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
			UE_LOG(LogTemp, Display, TEXT("LOADED MATERIAL: %s"), text);
			TMaterialInstances.Add(MaterialAsset.Object);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MISSING MATERIAL: % s"), text);
			PrimaryActorTick.bCanEverTick = false;
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
	if (GAME_STATE != GAME_OVER)
	{
		GAME_STATE = GAME_STATE ? GAME_RUNNING : GAME_PAUSED;
	}
}

void ATOTRISGameModeBase::BeginPlay()
{
	GAME_STATE = GAME_RUNNING;
	GenerateBoard();
	DrawPiece(6);
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
