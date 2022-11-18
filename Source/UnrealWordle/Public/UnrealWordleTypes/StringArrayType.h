#pragma once

#include "CoreMinimal.h"
#include "StringArrayType.generated.h"

USTRUCT(BlueprintType)
struct FStringArray
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> Strings;
};