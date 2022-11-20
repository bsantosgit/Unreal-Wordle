// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/GameOverWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/UnrealWordleGM.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(MainMenuButton)
		MainMenuButton->OnClicked.AddDynamic(this, &UGameOverWidget::MainMenuButtonClicked);
	if(QuitGameButton)
		QuitGameButton->OnClicked.AddDynamic(this, &UGameOverWidget::QuitGameButtonClicked);

}

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameModeRef = Cast<AUnrealWordleGM>(UGameplayStatics::GetGameMode(this));
	if(GameModeRef)
	{
		if(GoalWordText)
		{
			GoalWordText->SetText(FText::FromString(GameModeRef->GetGoalWord()));
		}
	}
}

void UGameOverWidget::MainMenuButtonClicked()
{
	RemoveFromParent();
	if(GameModeRef)
	{
		GameModeRef->ShowMainMenu();
	}
}

void UGameOverWidget::QuitGameButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Quit Game from Game Over Menu"));
}
