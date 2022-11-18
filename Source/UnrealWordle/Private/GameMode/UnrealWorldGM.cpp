// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/UnrealWorldGM.h"

#include "Utils/UnrealWordleLibrary.h"

void AUnrealWorldGM::BeginPlay()
{
	Super::BeginPlay();

	if(UUnrealWordleLibrary::LoadWordsFromFile(FString("words2.txt"), 3, Words))
	{
		ShowMainMenu();
	}
}

void AUnrealWorldGM::ShowMainMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("Show Main Menu"));
}
