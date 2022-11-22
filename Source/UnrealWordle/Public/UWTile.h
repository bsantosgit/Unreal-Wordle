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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UWTile", meta=(AllowPrivateAccess=True))
	UStaticMeshComponent* TileMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UWTile", meta=(AllowPrivateAccess=True))
	UWidgetComponent* WidgetFront;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UWTile", meta=(AllowPrivateAccess=True))
	UWidgetComponent* WidgetBack;
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="UWTile")
	void IncorrectLetterError();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="UWTile")
	void SubmitLetter(UMaterialInterface* Material);
	
	void ClearLetter();
	void SetLetter(FString Letter);
	FString GetLetter() const;
};
