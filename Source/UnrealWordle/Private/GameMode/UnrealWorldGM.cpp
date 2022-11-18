// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/UnrealWorldGM.h"

#include "Blueprint/UserWidget.h"
#include "HUD/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/UnrealWordleLibrary.h"

AUnrealWorldGM::AUnrealWorldGM()
{
	GoalWord = FString("GLIDE");
}

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
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(MainMenuWidgetClass == nullptr) return;
	MainMenuWidgetRef = CreateWidget<UMainMenuWidget>(PC, MainMenuWidgetClass);
	MainMenuWidgetRef->AddToViewport();
	MainMenuWidgetRef->bIsFocusable = true;
	FInputModeUIOnly InputModeUI;
	InputModeUI.SetWidgetToFocus(MainMenuWidgetRef->TakeWidget());
	InputModeUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(InputModeUI);
	PC->SetShowMouseCursor(true);
}

void AUnrealWorldGM::StartRound(int32 WordLength, int32 GuessCount)
{
	const FStringArray* WordFound = Words.Find(WordLength);
	if(WordFound != nullptr)
	{
		const int32 RandomWordIndex = FMath::RandRange(0, WordFound->Strings.Num());
		// Disabled For Testing
		// GoalWord = WordFound->Strings[RandomWordIndex];
		// For Testing
		GoalWord = "APPLE";
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *GoalWord);
}
