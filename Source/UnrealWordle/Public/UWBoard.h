// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UWBoard.generated.h"

class UTextRenderComponent;
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
	
public:
	UFUNCTION(BlueprintCallable, Category="WordleBoard")
	void DestroyBoardAndTiles();

};
