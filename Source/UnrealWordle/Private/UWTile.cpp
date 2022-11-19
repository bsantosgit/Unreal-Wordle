// Fill out your copyright notice in the Description page of Project Settings.


#include "UWTile.h"

#include "Components/WidgetComponent.h"

AUWTile::AUWTile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(SceneRoot);

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMesh->SetupAttachment(RootComponent);

	WidgetFront = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetFront"));
	WidgetFront->SetupAttachment(TileMesh);

	WidgetBack = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetBack"));
	WidgetBack->SetupAttachment(TileMesh);
}

void AUWTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AUWTile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUWTile::ClearLetter()
{
	UE_LOG(LogTemp, Warning, TEXT("Tile Cleared - %s"), *GetName());
}


