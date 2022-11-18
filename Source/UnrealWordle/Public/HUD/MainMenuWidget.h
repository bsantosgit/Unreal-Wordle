// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class UNREALWORDLE_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MainMenuWidget", meta=(BindWidget))
	UTextBlock* WordLengthText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MainMenuWidget", meta=(BindWidget))
	UTextBlock* GuessCountText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MainMenuWidget", meta=(BindWidget))
	UButton* DownWordLengthButton;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MainMenuWidget", meta=(BindWidget))
	UButton* UpWordLengthButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MainMenuWidget", meta=(BindWidget))
	UButton* DownGuessCountButton;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MainMenuWidget", meta=(BindWidget))
	UButton* UpGuessCountButton;

	virtual void NativeOnInitialized() override;

private:
	int32 WordLength;
	int32 GuessCount;

	UFUNCTION()
	void DownWordLengthButtonClicked();
	UFUNCTION()
	void UpWordLengthButtonClicked();
	UFUNCTION()
	void DownGuessCountButtonClicked();
	UFUNCTION()
	void UpGuessCountButtonClicked();

public:
	UFUNCTION()
	FText SetWordLengthText() const;
	UFUNCTION()
	FText SetGuessCountText() const;
};
