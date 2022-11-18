// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainMenuWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"


void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WordLength = 4;
	GuessCount = 4;

	if(DownWordLengthButton)
		DownWordLengthButton->OnClicked.AddDynamic(this, &UMainMenuWidget::DownWordLengthButtonClicked);
	if(UpWordLengthButton)
		UpWordLengthButton->OnClicked.AddDynamic(this, &UMainMenuWidget::UpWordLengthButtonClicked);
	if(DownGuessCountButton)
		DownGuessCountButton->OnClicked.AddDynamic(this, &UMainMenuWidget::DownGuessCountButtonClicked);
	if(UpGuessCountButton)
		UpGuessCountButton->OnClicked.AddDynamic(this, &UMainMenuWidget::UpGuessCountButtonClicked);

	if(WordLengthText)
	{
		WordLengthText->TextDelegate.BindUFunction(this, "SetWordLengthText");
		WordLengthText->SynchronizeProperties();
	}
	if(GuessCountText)
	{
		GuessCountText->TextDelegate.BindUFunction(this, "SetGuessCountText");
		GuessCountText->SynchronizeProperties();
	}
}

void UMainMenuWidget::DownWordLengthButtonClicked()
{
	if(WordLength > 3)
	{
		WordLength--;
	}
}

void UMainMenuWidget::UpWordLengthButtonClicked()
{
	if(WordLength < 10)
	{
		WordLength++;
	}
}

void UMainMenuWidget::DownGuessCountButtonClicked()
{
	if(GuessCount > 3)
	{
		GuessCount--;
	}
}

void UMainMenuWidget::UpGuessCountButtonClicked()
{
	if(GuessCount < 10)
	{
		GuessCount++;
	}
}

FText UMainMenuWidget::SetWordLengthText() const
{
	return FText::FromString(FString::Printf(TEXT("%d"), WordLength));
}

FText UMainMenuWidget::SetGuessCountText() const
{
	return FText::FromString(FString::Printf(TEXT("%d"), GuessCount));
}