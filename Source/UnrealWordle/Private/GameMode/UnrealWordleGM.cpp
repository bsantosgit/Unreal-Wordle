// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/UnrealWordleGM.h"

#include "UWBoard.h"
#include "UWTile.h"
#include "Blueprint/UserWidget.h"
#include "HUD/GameOverWidget.h"
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

TArray<ETileState> AUnrealWordleGM::PreSubmitTile(const FString& GuessWord)
{
	TArray<ETileState> Matches;
	TMap<FString, int32> ProvidedRemain;
	TMap<FString, int32> SolutionRemain;
	TMap<FString, int32> Reassignable;
	
	for(int32 i = 0; i < GuessWord.Len(); i++)
	{
		Matches.Add(ETileState::TS_Incorrect);
	}

	for(int32 i = 0; i < GuessWord.Len(); i++)
	{
		if(GuessWord[i] == GoalWord[i])
			Matches[i] = ETileState::TS_Perfect;
	}

	for(int32 i = 0; i < GuessWord.Len(); i++){
		if(Matches[i] != ETileState::TS_Perfect){
			FString SGuessWord(FString::Chr(GuessWord[i]));
			if(ProvidedRemain.Find(SGuessWord))
			{
				int32 Val = ProvidedRemain[SGuessWord] + 1;
				ProvidedRemain.Emplace(SGuessWord, Val);	
			}else
			{
				ProvidedRemain.Emplace(SGuessWord, 1);
			}
			
			FString SGoalWord(FString::Chr(GoalWord[i]));
			if(SolutionRemain.Find(SGoalWord))
			{
				int32 Val = SolutionRemain[SGoalWord] + 1;
				SolutionRemain.Emplace(SGoalWord, Val);	
			}else
			{
				SolutionRemain.Emplace(SGoalWord, 1);
			}
		}
	}

	for(auto PItem : ProvidedRemain)
	{
		int X = 0;
		if(SolutionRemain.Find(PItem.Key))
		{
			X = SolutionRemain[PItem.Key];
		}else
		{
			X = 0;
		}

		Reassignable.Emplace(PItem.Key,FMath::Min(PItem.Value, X));
	}

	for(int32 i = 0; i < GuessWord.Len(); i++)
	{
		FString SGuessWord(FString::Chr(GuessWord[i]));
		if(Reassignable.Find(SGuessWord))
		{
			if(Matches[i] != ETileState::TS_Perfect && Reassignable[SGuessWord] != 0)
			{
				Matches[i] = ETileState::TS_Correct;
				auto Val = Reassignable[SGuessWord] - 1;
				Reassignable.Emplace(SGuessWord, Val);
			}
		}
	}

	return Matches;
}

bool AUnrealWordleGM::CheckForGameOver()
{
	if(BoardRef == nullptr) return true;
	FString CurrentWord("");
	const bool IsCurrentWordValid = GetCurrentWord(CurrentWord);

	if(IsCurrentWordValid && GoalWord == CurrentWord)
		return true;

	if(CurrentGuessIndex >= BoardRef->GetGuessCount()-1)
		return true;

	return false;
}

void AUnrealWordleGM::EndGame()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(GameOverWidgetClass == nullptr) return;
	GameOverWidgetRef = CreateWidget<UGameOverWidget>(PC, GameOverWidgetClass);
	GameOverWidgetRef->bIsFocusable = true;
	
	FInputModeUIOnly InputModeUI;
	InputModeUI.SetWidgetToFocus(GameOverWidgetRef->TakeWidget());
	InputModeUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(InputModeUI);
	PC->SetShowMouseCursor(true);

	GameOverWidgetRef->AddToViewport();
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
		const TArray<ETileState> Matches = PreSubmitTile(CurrentWord);
		HandleValidWordSubmitted(Matches);
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

void AUnrealWordleGM::HandleValidWordSubmitted(const TArray<ETileState>& Matches)
{
	constexpr int32 SubmitWordIndex = 0;
	HandleNextTile(SubmitWordIndex, Matches);
}

void AUnrealWordleGM::HandleNextTile(int32 SubmitWordIndex, const TArray<ETileState>& Matches)
{
	if(BoardRef == nullptr) return;
	AUWTile* Tile = BoardRef->GetTile(CurrentGuessIndex, SubmitWordIndex);
	SubmitTile(Tile, SubmitWordIndex, Matches);
	if(SubmitWordIndex < BoardRef->GetWordLastValidIndex())
	{
		FTimerDelegate NextTileDelegate;
		SubmitWordIndex++;
		NextTileDelegate.BindUFunction(this, FName("HandleNextTile"), SubmitWordIndex, Matches);
		GetWorldTimerManager().SetTimer(TH_NextTile, NextTileDelegate, 0.4f, false);
	}else
	{
		GetWorldTimerManager().ClearTimer(TH_NextTile);
		if(CheckForGameOver())
		{
			FTimerHandle TH_EndGame;
			GetWorldTimerManager().SetTimer(TH_EndGame, this, &AUnrealWordleGM::EndGame, 1.0f);
		}else
		{
			ResetInputModeToGame();
			CurrentGuessIndex++;
			CurrentLetterIndex = 0;	
		}
	}
}

void AUnrealWordleGM::SubmitTile(AUWTile* CurrentTile, int32 CurrentIndex, const TArray<ETileState>& Matches)
{
	// https://codereview.stackexchange.com/questions/278966/correct-logic-for-string-comparison-in-wordle
	// Logic Based on Above Link
	switch (Matches[CurrentIndex]) {
		case ETileState::TS_Perfect: CurrentTile->SubmitLetter(LetterPerfect); break;
		case ETileState::TS_Incorrect: CurrentTile->SubmitLetter(LetterInCorrect); break;
		case ETileState::TS_Correct: CurrentTile->SubmitLetter(LetterCorrect); break;
		default: checkNoEntry();
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
		GoalWord = WordFound->Strings[RandomWordIndex];
		// For Testing
		// GoalWord = "APPLE";

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
