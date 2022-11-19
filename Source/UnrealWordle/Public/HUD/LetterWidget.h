// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LetterWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class UNREALWORDLE_API ULetterWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="LetterWidget",meta=(BindWidget))
	UTextBlock* LetterText;

	void SetLetter(FText Letter);
	
protected:
	virtual void NativeOnInitialized() override;
};
