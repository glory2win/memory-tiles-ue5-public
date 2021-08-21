// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MemoryTilesGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MEMORYTILES_UE5_API AMemoryTilesGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:

	AMemoryTilesGameMode();
	
	/** Properties */


	UPROPERTY(EditAnywhere, Category = "Board")
	TSubclassOf<class ABoard> BoardPrefab;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "References")
	class ABoard* BoardRef;


public:

	UFUNCTION(BlueprintCallable, Category= "Game")
	void RestartGame() const;

	
};
