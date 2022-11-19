// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UnrealWordleTypes/StringArrayType.h"
#include "UnrealWordleLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UNREALWORDLE_API UUnrealWordleLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="UnrealWordleLibrary")
	static bool LoadWordsFromFile(FString FileName, int32 RequiredWordLength, TMap<int32, FStringArray>& Words);

	UFUNCTION(BlueprintCallable, Category="UnrealWordleLibrary")
	static bool IsLetter(FString String);

private:
	static bool IsASCIILetter(char c);
};
