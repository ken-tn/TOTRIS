// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"
#include <TOTRIS/TOTRISGameModeBase.h>

// Sets default values
ACube::ACube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetActorEnableCollision(false);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent->SetMobility(EComponentMobility::Movable);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionProfileName(FName("NoCollision"), false);
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->CastShadow = false;
}

void ACube::Init(FVector2D pos, UMaterialInstanceConstant* Material, UStaticMesh* CubeMesh)
{
	x = pos.X;
	y = pos.Y;
	
	RootComponent->SetWorldTransform(FTransform(FVector(0, x * 200, (ATOTRISGameModeBase::BOARD_HEIGHT - y) * ATOTRISGameModeBase::CUBE_SIZE)));
	StaticMesh->SetStaticMesh(CubeMesh);
	StaticMesh->SetMaterial(0, Material);
}

// Called when the game starts or when spawned
void ACube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

