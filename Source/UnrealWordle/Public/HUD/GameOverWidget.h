// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

class UTextBlock;
class UButton;
class AUnrealWordleGM;

/**
 * 
 */
UCLASS()
class UNREALWORDLE_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GameOverWidget", meta=(BindWidget))
	UTextBlock* GoalWordText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GameOverWidget", meta=(BindWidget))
	UButton* MainMenuButton;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GameOverWidget", meta=(BindWidget))
	UButton* QuitGameButton;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void MainMenuButtonClicked();
	UFUNCTION()
	void QuitGameButtonClicked();

	AUnrealWordleGM* GameModeRef;	
public:
};
