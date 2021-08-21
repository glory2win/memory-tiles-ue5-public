// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "MemoryTilesGameMode.h"
#include "Engine/DataTable.h"
#include "MemoryTilesPlayerController.h"
#include "Board.generated.h"


#pragma region BlockGrid
UCLASS()
class UBlockGrid final : public UObject {
	GENERATED_BODY()

public:
	UBlockGrid();

	UFUNCTION()
	void Init(const uint32 SizeOfGrid);

	ATile* GetTileAt(const uint32 Row, const uint32 Column) const;

	UFUNCTION()
	void Add(ATile* Tile);

	FORCEINLINE int32 GetGridLength() const { return TileArray.Num(); }

	FORCEINLINE ATile* GetTileAtIndex(const int32 Index) const { return TileArray[Index]; }

private:
	UPROPERTY()
	TArray<ATile*> TileArray;

	UPROPERTY()
	uint32 Size;
	
};

#pragma endregion BlockGrid



#pragma region Board
UCLASS()
class MEMORYTILES_UE5_API ABoard : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	/** Properties*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MemoryTiles - Prefabs")
	TSubclassOf<class ATile> TilePrefab; // Pick BP_Tile from Blueprints, that will be instantiated at runtime.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MemoryTiles - Prefabs")
	TSubclassOf<class AActor> BlockedPrefab; // Red color non-interactive Block.

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "MemoryTiles - References")
	AMemoryTilesGameMode* GameModeRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MemeoryTiles - Grid")
	uint8 Size;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MemoryTiles - Grid")
	float TileSpacing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MemoryTiles - Gameplay")
	int32 MaxNumOfTries; // Maximum tries count before game end.


	bool bIsBusy;

	/** Functions*/
	void AddToSelectionArray(ATile* SelectedTile);

	void CheckBoardForMatch();

	void ShuffleBoard();

	void CheckRemainTries();

	void CheckRemainTiles();

	virtual void Reset() override;


private:
	
	UPROPERTY()
	UBlockGrid* Grid;

	UPROPERTY()
	TArray<ATile*> SelectedTilesArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data Table", meta = (AllowPrivateAccess = "true"))
	UDataTable* TileDataTable;

	UPROPERTY()
	AMemoryTilesPlayerController* PlayerController;


	bool bIsFreshMatch;

	int32 TotalMatchCount;
	int32 NumberOfTries;
	int32 RemainTiles;


	FTimerHandle MatchSuccessTimerHandle;




	/** FUNCTIONS */

	UFUNCTION()
	void CreateBoard();

};
#pragma endregion Board
