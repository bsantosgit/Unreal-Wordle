#pragma once

#include "CoreMinimal.h"
#include "TileStateType.generated.h"

UENUM(BlueprintType)
enum class ETileState : uint8
{
	TS_Perfect UMETA(DisplayName="Perfect"),
	TS_Incorrect UMETA(DisplayName="Incorrect"),
	TS_Correct UMETA(DisplayName="Correct"),

	TS_MAX UMETA(DisplayName="DefaultMAX")
};