// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInstanceConstant.h"
#include "GameFramework/GameModeBase.h"
#include "TOTRISGameModeBase.generated.h"

#define Piece TArray<AActor*>
#define Shape TArray<TArray<int>>

/**
 * 
 */
UCLASS()
class TOTRIS_API ATOTRISGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	int FRAMES_PER_TICK = 30;
	TArray<int> BOARD_SIZE = { 10, 20 };
	TArray<TArray<int>> BOARD;
	Piece CURRENTPIECE;

	int frame = 0;
	UPROPERTY(VisibleAnywhere)
		int CubeSize = 200;
	UStaticMesh* UCubeMesh;

	TArray<Shape> tetris_shapes =
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

	TArray<FString> TMaterialReferences = {
		FString("MaterialInstanceConstant'/Game/AutomotiveMaterials/Materials/Exterior/CarPaint/MI_CarPaint_Red.MI_CarPaint_Red'"),
		FString("MaterialInstanceConstant'/Game/AutomotiveMaterials/Materials/Exterior/CarPaint/MI_CarPaint_Orange.MI_CarPaint_Orange'"),
		FString("MaterialInstanceConstant'/Game/AutomotiveMaterials/Materials/Exterior/CarPaint/MI_CarPaint_LightBlue.MI_CarPaint_LightBlue'"),
		FString("MaterialInstanceConstant'/Game/AutomotiveMaterials/Materials/Exterior/CaliperPaint/MI_Caliper_Yellow.MI_Caliper_Yellow'"),
		FString("MaterialInstanceConstant'/Game/AutomotiveMaterials/Materials/Exterior/CarPaint/MI_CarPaint_White.MI_CarPaint_White'"),
		FString("MaterialInstanceConstant'/Game/AutomotiveMaterials/Materials/Interior/CarbonFiber/MI_CarbonFiber.MI_CarbonFiber'"),
		FString("MaterialInstanceConstant'/Game/AutomotiveMaterials/Materials/Exterior/CarPaint/MI_CarPaint_Mix_02.MI_CarPaint_Mix_02'")
	};

	TArray<UMaterialInstance*> TMaterialInstances;

	void GenerateBoard();
	void GameTick();
	void DrawPiece();
	
	AActor* DrawCube(int x, int y, int colour);

public:
	void RotateClockwise();

public:
	ATOTRISGameModeBase();

	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
