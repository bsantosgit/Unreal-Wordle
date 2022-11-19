// Fill out your copyright notice in the Description page of Project Settings.


#include "UWTile.h"

#include "Components/WidgetComponent.h"
#include "HUD/LetterWidget.h"

AUWTile::AUWTile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(SceneRoot);

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMesh->SetupAttachment(RootComponent);
	TileMesh->CastShadow = false;

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
	SetLetter(TEXT(""));
}

void AUWTile::SetLetter(FString Letter)
{
	ULetterWidget* FrontLetterWidget = Cast<ULetterWidget>(WidgetFront->GetUserWidgetObject());
	if(FrontLetterWidget)
	{
		FrontLetterWidget->SetLetter(FText::FromString(Letter));
	}
	ULetterWidget* BackLetterWidget = Cast<ULetterWidget>(WidgetBack->GetUserWidgetObject());
	if(BackLetterWidget)
	{
		BackLetterWidget->SetLetter(FText::FromString(Letter));
	}
}


