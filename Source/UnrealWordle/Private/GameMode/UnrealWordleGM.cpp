// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/UnrealWordleGM.h"

#include "UWBoard.h"
#include "Blueprint/UserWidget.h"
#include "HUD/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/UnrealWordleLibrary.h"

AUnrealWordleGM::AUnrealWordleGM()
{
	GoalWord = FString("GLIDE");
	WordsFileName = FString("words2.txt");
	CurrentGuessIndex = 0;
	CurrentLetterIndex = 0;
}

void AUnrealWordleGM::BeginPlay()
{
	Super::BeginPlay();

	if(UUnrealWordleLibrary::LoadWordsFromFile(WordsFileName, 3, Words))
	{
		ShowMainMenu();
	}
}

void AUnrealWordleGM::ShowMainMenu()
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

void AUnrealWordleGM::StartRound(int32 WordLength, int32 GuessCount)
{
	const FStringArray* WordFound = Words.Find(WordLength);
	if(WordFound != nullptr)
	{
		const int32 RandomWordIndex = FMath::RandRange(0, WordFound->Strings.Num());
		// Disabled For Testing
		// GoalWord = WordFound->Strings[RandomWordIndex];
		// For Testing
		GoalWord = "APPLE";

		CurrentGuessIndex = 0;
		CurrentLetterIndex = 0;
		if(IsValid(MainMenuWidgetRef))
		{
			MainMenuWidgetRef->RemoveFromParent();
			MainMenuWidgetRef = nullptr;
		}

		const APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(this, 0);
		if(PCM == nullptr) return;

		FTransform BoardSpawnTransform;
		const FVector CameraForwardDirection(FRotationMatrix(PCM->GetCameraRotation()).GetUnitAxis(EAxis::X));
		BoardSpawnTransform.SetLocation(PCM->GetCameraLocation() + (CameraForwardDirection * 1000.f));
		BoardSpawnTransform.SetRotation(FQuat(FRotator(0.f, 180.f, 0.f)));
		FActorSpawnParameters BoardSpawnParams;
		BoardSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		BoardRef = GetWorld()->SpawnActor<AUWBoard>(BoardClass, BoardSpawnTransform, BoardSpawnParams);

		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);

		if(IsValid(BoardRef))
		{
			BoardRef->DestroyBoardAndTiles();
			BoardRef = nullptr;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *GoalWord);
}
