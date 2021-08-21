// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MemoryTilesPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MEMORYTILES_UE5_API AMemoryTilesPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMemoryTilesPlayerController();


	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void ShowGameSuccessHUD();

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void ShowGameFailHUD();

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void UpdateRemainMovesText(int32 RemainMovesCount);
};
