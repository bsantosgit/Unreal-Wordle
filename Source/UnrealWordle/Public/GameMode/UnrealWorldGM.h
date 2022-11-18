// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UnrealWordleTypes/StringArrayType.h"
#include "UnrealWorldGM.generated.h"

/**
 * 
 */
UCLASS()
class UNREALWORDLE_API AUnrealWorldGM : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

private:
	void ShowMainMenu();
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="UnrealGameMode")
	TMap<int32, FStringArray> Words;
};
