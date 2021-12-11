// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "Cube.generated.h"

UCLASS()
class TOTRIS_API ACube : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACube();

	UPROPERTY(VisibleAnywhere)
		int x;
	UPROPERTY(VisibleAnywhere)
		int y;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* StaticMesh;

	void Init(FVector2D pos, UMaterialInstanceConstant* Material, UStaticMesh* CubeMesh);

protected:
	void init();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
