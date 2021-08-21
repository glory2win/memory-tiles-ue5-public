// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Tile.generated.h"

USTRUCT(BlueprintType)
struct FTileData : public FTableRowBase {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	int32 TileMatchId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	UMaterialInstance* TileMaterial; // Assign this to OpenSpriteMaterial in Tile.
};


UCLASS()
class MEMORYTILES_UE5_API  ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	/** Properties*/
	
	UPROPERTY(EditAnywhere, Category = "References")
	UMaterialInstance* OpenSpriteMaterial;

	UPROPERTY(EditAnywhere, Category = "References")
	UMaterialInstance* CloseSpriteMaterial;

	UPROPERTY(EditAnywhere, Category = "References")
	UMaterialInstance* BlockerSpriteMaterial;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "References")
	class ABoard* BoardRef;	


	FTimerHandle MatchSuccessTimerHandle;
	FTimerHandle MatchFailTimerHandle;
	

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MatchItemMesh;

	UPROPERTY()
	uint32 TileMatchID;

	bool bIsAvailable;

	


	/** Functions */

	UFUNCTION()
	void TileClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	UFUNCTION()
	void Open() const;

	UFUNCTION()
	void Close() const;


	// UFUNCTION()
	// FActorDestroyedSignature OnTileDestroy();

public:

	UFUNCTION()
	uint32 GetTileMatchID() const { return TileMatchID; }

	UFUNCTION()
	void Hide();

	UFUNCTION()
	void ShakeAndClose();


	void OnMatchSuccess();
	void OnMatchFail();
	
	void SetAsBlockerTile();

	void SetTileProperties(const FTileData& TileData);

	virtual void Reset() override;

	void ToggleVisibility(bool bShow) const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Tile")
	void PlayShakeAnim(); // Event that is used in Blueprint to call Timeline Shake Animation.

	UFUNCTION(BlueprintCallable, Category = "Tile")
	void AfterShakeAnim(); // This function will be called by the Timeline Finished Event.


};

