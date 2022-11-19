// Fill out your copyright notice in the Description page of Project Settings.


#include "UWBoard.h"

#include "UWTile.h"
#include "Components/TextRenderComponent.h"

AUWBoard::AUWBoard()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(SceneRoot);
	
	HeadingTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HeadingTextRender"));
	HeadingTextRender->SetupAttachment(RootComponent);
	HeadingTextRender->Text = FText::FromString(TEXT("WORDLE"));
	HeadingTextRender->WorldSize = 80.f;
	HeadingTextRender->HorizontalAlignment = EHorizTextAligment::EHTA_Center;

	BoardTopMiddleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("BoardTopMiddleLocation"));
	BoardTopMiddleLocation->SetupAttachment(RootComponent);

	TileSize = 100.f;
	PaddingSize = 10.f;
}

void AUWBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUWBoard::BeginPlay()
{
	Super::BeginPlay();
	InitializeBoard();
}

void AUWBoard::InitializeBoard()
{
	// _GC -> Row & _WL -> Column
	for(int32 _GC = 0; _GC < GuessCount; _GC++)
	{
		for(int32 _WL = 0; _WL < WordLength; _WL++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%d - %d"), _GC, _WL);
			FTransform TileTransform;
			const float TileYOffset = (_WL - WordLength/2) * (TileSize + PaddingSize);
			const float TileZOffset = _GC * (TileSize + PaddingSize) * -1.f;
			TileTransform.SetLocation(BoardTopMiddleLocation->GetComponentLocation() + FVector(0.f, TileYOffset, TileZOffset));
			TileTransform.SetRotation(FQuat(BoardTopMiddleLocation->GetComponentRotation()));

			FActorSpawnParameters TileSpawnParams;
			TileSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			if(TileClass == nullptr) return;
			AUWTile* Tile = GetWorld()->SpawnActor<AUWTile>(TileClass, TileTransform, TileSpawnParams);
			Tile->ClearLetter();
			Tiles.Add(Tile);
		}
	}
}

void AUWBoard::DestroyBoardAndTiles()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString("Destory Board and Tiles Called"));
}

AUWTile* AUWBoard::GetTile(int32 GuessIndex, int32 LetterIndex) const
{
	return Tiles[(WordLength * GuessIndex) + LetterIndex];
}


