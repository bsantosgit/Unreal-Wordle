// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UWTile.generated.h"

class UStaticMeshComponent;
class UWidgetComponent;

UCLASS()
class UNREALWORDLE_API AUWTile : public AActor
{
	GENERATED_BODY()
	
public:	
	AUWTile();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category="Misc")
	USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere, Category="UWTile")
	UStaticMeshComponent* TileMesh;
	UPROPERTY(VisibleAnywhere, Category="UWTile")
	UWidgetComponent* WidgetFront;
	UPROPERTY(VisibleAnywhere, Category="UWTile")
	UWidgetComponent* WidgetBack;
	
public:	
	void ClearLetter();
	void SetLetter(FString Letter);
	FString GetLetter() const;
};
