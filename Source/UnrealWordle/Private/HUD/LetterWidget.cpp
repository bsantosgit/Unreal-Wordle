// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/LetterWidget.h"

#include "Components/TextBlock.h"

void ULetterWidget::SetLetter(FText Letter)
{
	if(LetterText)
	{
		LetterText->SetText(Letter);
	}
}

FText ULetterWidget::GetLetter() const
{
	return LetterText->GetText();
}

void ULetterWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if(LetterText)
	{
		LetterText->SetText(FText::FromString(TEXT("R")));
	}
}
