// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Cube.h"
#include "GameFramework/GameModeBase.h"
#include "TOTRISGameModeBase.generated.h"

#define Piece TArray<ACube*>
#define Shape TArray<TArray<int>>
#define GAME_RUNNING 0
#define GAME_PAUSED 1
#define GAME_OVER 2

/**
 * 
 */
UCLASS()
class TOTRIS_API ATOTRISGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	int FRAMES_PER_TICK = 25;
	static const int BOARD_WIDTH = 10;
	static const int BOARD_HEIGHT = 20;
	static const int CUBE_SIZE = 200;
	int GAME_STATE;

private:
	TArray<TArray<int>> BOARD;
	TArray<TArray<ACube*>> BOARDCUBES;
	Piece CURRENTPIECE;
	int frame = 0;

	const TArray<Shape> tetris_shapes =
	{
		{{1, 1, 1},
		 {0, 1, 0}},

		{{0, 2, 2},
		 {2, 2, 0}},

		{{3, 3, 0},
		 {0, 3, 3}},

		{{4, 0, 0},
		 {4, 4, 4}},

		{{0, 0, 5},
		 {5, 5, 5}},

		{{6, 6, 6, 6}},

		{{7, 7},
		 {7, 7}}
	};

	const TArray<TArray<FVector2D>> NESOffsets = { 
		{FVector2D(-1,1), FVector2D(0,-1), FVector2D(-1,0), FVector2D(0,0)},
		{FVector2D(-1,1), FVector2D(0,-1), FVector2D(-1,1), FVector2D(0,-1)},
		{FVector2D(-2,1), FVector2D(1,-1), FVector2D(-2,1), FVector2D(1,-1)},
		{FVector2D(-1,0), FVector2D(1,0), FVector2D(-1,1), FVector2D(0,-1)},
		{FVector2D(0,0), FVector2D(-1,0), FVector2D(-1,1), FVector2D(0,-1)},
		{FVector2D(-2,2), FVector2D(2,-2), FVector2D(-2,2), FVector2D(2,-2)},
		{FVector2D(0,0), FVector2D(0,0), FVector2D(0,0), FVector2D(0,0)}
	};

	const TArray<FString> TMaterialReferences = {
		FString("MaterialInstanceConstant'/Game/Material/One.One'"),
		FString("MaterialInstanceConstant'/Game/Material/Two.Two'"),
		FString("MaterialInstanceConstant'/Game/Material/Three.Three'"),
		FString("MaterialInstanceConstant'/Game/Material/Four.Four'"),
		FString("MaterialInstanceConstant'/Game/Material/Five.Five'"),
		FString("MaterialInstanceConstant'/Game/Material/Six.Six'"),
		FString("MaterialInstanceConstant'/Game/Material/Seven.Seven'")
	};

	UStaticMesh* UCubeMesh;
	UPROPERTY(VisibleAnywhere)
		TArray<UMaterialInstanceConstant*> TMaterialInstances;

	bool IsCollision(FVector2D Pos);
	bool IsPieceColliding(FVector2D PosOffset);
	void GenerateBoard();
	void GameTick();
	UFUNCTION()
		void DrawPiece(const int& shapeIndex = FMath::RandRange(0, 6));
	Shape RotateMatrix(Shape shape);
	
	ACube* DrawCube(int x, int y, int colour);

public:
	void TogglePause();
	void RotateClockwise();
	bool Move(FVector2D Offset);

public:
	ATOTRISGameModeBase();

	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
