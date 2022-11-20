// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/UnrealWordleGM.h"

#include "UWBoard.h"
#include "UWTile.h"
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

void AUnrealWordleGM::OnLetterTyped(FString Letter)
{
	if(BoardRef == nullptr) return;
	if(CurrentLetterIndex < BoardRef->GetWordLength())
	{
		AUWTile* Tile = BoardRef->GetTile(CurrentGuessIndex, CurrentLetterIndex);
		Tile->SetLetter(Letter);
		EditCurrentLetterIndex(1);
	}
}

void AUnrealWordleGM::OnBackspaceTyped()
{
	if(BoardRef == nullptr) return;
	EditCurrentLetterIndex(-1);
	AUWTile* Tile = BoardRef->GetTile(CurrentGuessIndex, CurrentLetterIndex);
	if(Tile)
	{
		Tile->ClearLetter();
	}
	
}

void AUnrealWordleGM::SubmitWord()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FInputModeUIOnly InputModeUI;
	InputModeUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(InputModeUI);
	FString CurrentWord("");
	if(GetCurrentWord(CurrentWord))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, FString("VALID"));
		HandleValidWordSubmitted();
	}else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Orange, FString("INVALID"));
		if(BoardRef == nullptr) return;
		for(int32 Index = 0; Index <= BoardRef->GetWordLastValidIndex(); Index++)
		{
			AUWTile* Tile = BoardRef->GetTile(CurrentGuessIndex, Index);
			Tile->IncorrectLetterError();
		}
		FTimerHandle TH_TileInvalid;
		GetWorldTimerManager().SetTimer(TH_TileInvalid, this, &AUnrealWordleGM::ResetInputModeToGame, 0.75f, false);
		// GetWorldTimerManager().ClearTimer(TH_TileInvalid);
	}
}

void AUnrealWordleGM::ResetInputModeToGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Reseting Input to Game"));
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PC->SetInputMode(FInputModeGameOnly());
}

bool AUnrealWordleGM::GetCurrentWord(FString& CurrentWord)
{
	if(BoardRef == nullptr)
	{
		CurrentWord = FString("");
		return false;
	}
	FString SubmittedWord("");
	for(int32 Index = 0; Index <= BoardRef->GetWordLastValidIndex(); Index++)
	{
		const AUWTile* Tile = BoardRef->GetTile(CurrentGuessIndex, Index);
		if(!UUnrealWordleLibrary::IsLetter(Tile->GetLetter()))
		{
			CurrentWord = FString("");
			return false;
		}else
		{
			SubmittedWord = SubmittedWord.Append(Tile->GetLetter());
		}
	}

	// We Can Perform Binary Search as Strings array will be Alphabetically Sorted
	// Algo::BinarySearch()
	CurrentWord = SubmittedWord;
	return Words.Find(BoardRef->GetWordLength())->Strings.Contains(SubmittedWord);
}

void AUnrealWordleGM::HandleValidWordSubmitted()
{
	int32 SubmitWordIndex = 0;
	HandleNextTile(SubmitWordIndex);
}

void AUnrealWordleGM::HandleNextTile(int32 SubmitWordIndex)
{
	if(BoardRef == nullptr) return;
	AUWTile* Tile = BoardRef->GetTile(CurrentGuessIndex, SubmitWordIndex);
	SubmitTile(Tile, SubmitWordIndex);
	if(SubmitWordIndex < BoardRef->GetWordLastValidIndex())
	{
		FTimerDelegate NextTileDelegate;
		SubmitWordIndex++;
		NextTileDelegate.BindUFunction(this, FName("HandleNextTile"), SubmitWordIndex);
		GetWorldTimerManager().SetTimer(TH_NextTile, NextTileDelegate, 0.4f, false);
	}else
	{
		ResetInputModeToGame();
		CurrentGuessIndex++;
		CurrentLetterIndex = 0;
		GetWorldTimerManager().ClearTimer(TH_NextTile);
	}
}

void AUnrealWordleGM::SubmitTile(AUWTile* CurrentTile, int32 CurrentIndex)
{
	// TODO : Fix Bug - Duplicate Letter Second Index Not Registered
	FString CurrentTileLetter = CurrentTile->GetLetter();
	const int32 LetterFoundIndex = GoalWord.Find(CurrentTileLetter);
	if(LetterFoundIndex > -1)
	{
		// True if Letter is anywhere Inside the Word
		// if(LetterFoundIndex == CurrentIndex) // This Line Causes Bug for words having duplicate Letters
		if(GoalWord[CurrentIndex] == CurrentTileLetter[0])
		{
			// If In exact right Position
			CurrentTile->SubmitLetter(LetterPerfect);
		}else
		{
			CurrentTile->SubmitLetter(LetterCorrect);
		}
	}else
	{
		// Letter not in word
		CurrentTile->SubmitLetter(LetterInCorrect);
	}
}

void AUnrealWordleGM::EditCurrentLetterIndex(int32 Amount)
{
	CurrentLetterIndex = FMath::Clamp(CurrentLetterIndex + Amount, 0, BoardRef->GetWordLength());
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

		// This by default calls the Construction Script and Hence Setting any values Post Construction misses the Construction and Begin Play
		// So We Deferred the Actor Spawn, call to Constructor
		// BoardRef = GetWorld()->SpawnActor<AUWBoard>(BoardClass, BoardSpawnTransform, BoardSpawnParams);
		BoardRef = Cast<AUWBoard>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BoardClass, BoardSpawnTransform));

		if(IsValid(BoardRef))
		{
			BoardRef->SetWordLength(WordLength);
			BoardRef->SetGuessCount(GuessCount);
			UGameplayStatics::FinishSpawningActor(BoardRef, BoardSpawnTransform);
		}
		
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);

		if(IsValid(BoardRef))
		{
			BoardRef->DestroyBoardAndTiles();
			// BoardRef = nullptr;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *GoalWord);
}
