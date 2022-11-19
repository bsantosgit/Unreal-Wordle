// Fill out your copyright notice in the Description page of Project Settings.


#include "UWBoard.h"

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
}

void AUWBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUWBoard::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUWBoard::DestroyBoardAndTiles()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString("Destory Board and Tiles Called"));
}


