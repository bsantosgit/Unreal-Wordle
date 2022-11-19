// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UnrealWordleTypes/StringArrayType.h"
#include "UnrealWordleGM.generated.h"

class UUserWidget;
class UMainMenuWidget;
class AUWBoard;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UnrealGameMode")
	TSubclassOf<AUWBoard> BoardClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UnrealGameMode")
	UMainMenuWidget* MainMenuWidgetRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UnrealGameMode")
	AUWBoard* BoardRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UnrealGameMode")
	FString GoalWord;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UnrealGameMode")
	FString WordsFileName;

private:
	void ShowMainMenu();

	int32 CurrentGuessIndex;
	int32 CurrentLetterIndex;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="UnrealGameMode")
	TMap<int32, FStringArray> Words;

	UFUNCTION(Category="UnrealGameMode")
	void OnLetterTyped(FString Letter);
	void OnBackspaceTyped();
	void SubmitWord();
	void ResetInputModeToGame();
	bool GetCurrentWord(FString& CurrentWord);

	void EditCurrentLetterIndex(int32 Amount);

	void StartRound(int32 WordLength, int32 GuessCount);
};
