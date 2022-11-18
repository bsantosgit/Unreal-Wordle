// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UnrealWordleTypes/StringArrayType.h"
#include "UnrealWorldGM.generated.h"

class UUserWidget;
class UMainMenuWidget;

/**
 * 
 */
UCLASS()
class UNREALWORDLE_API AUnrealWorldGM : public AGameModeBase
{
	GENERATED_BODY()
public:
	AUnrealWorldGM();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UnrealGameMode")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UnrealGameMode")
	UMainMenuWidget* MainMenuWidgetRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UnrealGameMode")
	FString GoalWord;

private:
	void ShowMainMenu();
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="UnrealGameMode")
	TMap<int32, FStringArray> Words;

	void StartRound(int32 WordLength, int32 GuessCount);
};
