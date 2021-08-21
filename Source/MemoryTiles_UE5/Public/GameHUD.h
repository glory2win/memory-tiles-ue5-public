// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class MEMORYTILES_UE5_API UGameHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameSuccessScreen();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameFailScreen();

	/*
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateRemainMoves(int32 RemainMoves);
*/
};
