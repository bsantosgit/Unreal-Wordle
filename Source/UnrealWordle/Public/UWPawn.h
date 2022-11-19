// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UWPawn.generated.h"

class UCameraComponent;
class AUnrealWordleGM;

UCLASS()
class UNREALWORDLE_API AUWPawn : public APawn
{
	GENERATED_BODY()

public:
	AUWPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category="Misc")
	USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WordlePawn", meta=(AllowPrivateAccess=True))
	UCameraComponent* ViewCamera;

	void AnyKeyPressed(FKey Key);
	void BackspaceKeyPressed(FKey Key);
	void EnterKeyPressed(FKey Key);

	AUnrealWordleGM* UnrealWordleGMRef;
	
public:	

};
