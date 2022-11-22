// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UnrealWordleTypes/StringArrayType.h"
#include "UnrealWordleTypes/TileStateType.h"
#include "UnrealWordleGM.generated.h"

class UUserWidget;
class UMainMenuWidget;
class UGameOverWidget;
class AUWBoard;
class AUWTile;

/**
 * 
 */
UCLASS()
class UNREALWORDLE_API AUnrealWordleGM : public AGameModeBase
{
	GENERATED_BODY()
public:
	AUnrealWordleGM();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UnrealGameMode")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UnrealGameMode")
	UMainMenuWidget* MainMenuWidgetRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UnrealGameMode")
	TSubclassOf<AUWBoard> BoardClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UnrealGameMode")
	AUWBoard* BoardRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UnrealGameMode")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UnrealGameMode")
	UGameOverWidget* GameOverWidgetRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UnrealGameMode")
	FString GoalWord;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UnrealGameMode")
	FString WordsFileName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UnrealGameMode")
	UMaterialInterface* LetterInCorrect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UnrealGameMode")
	UMaterialInterface* LetterCorrect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UnrealGameMode")
	UMaterialInterface* LetterPerfect;

private:
	TArray<ETileState> PreSubmitTile(const FString& GuessWord);
	bool CheckForGameOver();
	void EndGame();

	int32 CurrentGuessIndex;
	int32 CurrentLetterIndex;
	FTimerHandle TH_NextTile;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="UnrealGameMode")
	TMap<int32, FStringArray> Words;

	void ShowMainMenu();
	UFUNCTION(Category="UnrealGameMode")
	void OnLetterTyped(FString Letter);
	void OnBackspaceTyped();
	void SubmitWord();
	void ResetInputModeToGame();
	bool GetCurrentWord(FString& CurrentWord);
	void HandleValidWordSubmitted(const TArray<ETileState>& Matches);
	UFUNCTION()
	void HandleNextTile(int32 SubmitWordIndex, const TArray<ETileState>& Matches);
	void SubmitTile(AUWTile* CurrentTile, int32 CurrentIndex, const TArray<ETileState>& Matches);

	void EditCurrentLetterIndex(int32 Amount);

	void StartRound(int32 WordLength, int32 GuessCount);

	FORCEINLINE FString GetGoalWord() const { return GoalWord; }
};
