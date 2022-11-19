// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UWBoard.generated.h"

class UTextRenderComponent;
class AUWTile;

UCLASS()
class UNREALWORDLE_API AUWBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	AUWBoard();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category="Misc")
	USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere, Category="WordleBoard")
	UTextRenderComponent* HeadingTextRender;
	UPROPERTY(VisibleAnywhere, Category="WordleBoard")
	USceneComponent* BoardTopMiddleLocation;

	UPROPERTY(EditDefaultsOnly, Category="WordleBoard")
	TSubclassOf<AUWTile> TileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WordleBoard", meta=(ExposeOnSpawn=True, AllowPrivateAccess=True))
	int32 WordLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WordleBoard", meta=(ExposeOnSpawn=True, AllowPrivateAccess=True))
	int32 GuessCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WordleBoard", meta=(AllowPrivateAccess=True))
	float TileSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WordleBoard", meta=(AllowPrivateAccess=True))
	float PaddingSize;

	TArray<AUWTile*> Tiles;
	
	void InitializeBoard();
	
public:
	UFUNCTION(BlueprintCallable, Category="WordleBoard")
	void DestroyBoardAndTiles();

	AUWTile* GetTile(int32 GuessIndex, int32 LetterIndex) const;

	FORCEINLINE int32 GetWordLength() const { return WordLength; }
	FORCEINLINE int32 GetWordLastValidIndex() const { return (WordLength - 1); }
	FORCEINLINE int32 GetGuessCount() const { return GuessCount; }
	FORCEINLINE void SetWordLength(int32 WL) { WordLength = WL; }
	FORCEINLINE void SetGuessCount(int32 GC) { GuessCount = GC; }

};
