// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Board.h"

// Sets default values
ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = false;

	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRoot"));
	RootComponent = DummyRoot;

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMesh->SetupAttachment(DummyRoot);
	TileMesh->OnClicked.AddDynamic(this, &ATile::TileClicked);

	MatchItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MatchItemMesh"));
	MatchItemMesh->SetupAttachment(TileMesh);	

	bIsAvailable = true;

	// OnDestroyed.AddDynamic(this, &ATile::OnTileDestroy, this);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	Close();
}

void ATile::TileClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked) {
	if (!bIsAvailable || BoardRef->bIsBusy) return;;
	Open();
	BoardRef->AddToSelectionArray(this);
	bIsAvailable = false;
	UE_LOG(LogTemp, Warning, TEXT("Ticle clicked, Id: %d"), TileMatchID);
}

void ATile::Open() const {
	TileMesh->SetMaterial(0, OpenSpriteMaterial);
	MatchItemMesh->SetVisibility(true);
}

void ATile::Close() const {
	TileMesh->SetMaterial(0, CloseSpriteMaterial);
	MatchItemMesh->SetVisibility(false);
}

void ATile::OnMatchSuccess() {
	GetWorldTimerManager().SetTimer(MatchSuccessTimerHandle, this, &ATile::Hide, 1.f);
}

void ATile::OnMatchFail() {
	GetWorldTimerManager().SetTimer(MatchFailTimerHandle, this, &ATile::ShakeAndClose, 1.f);	
}


void ATile::SetAsBlockerTile() {
	TileMesh->SetMaterial(0, BlockerSpriteMaterial);
	MatchItemMesh->SetVisibility(false);
	bIsAvailable = false;
}

void ATile::SetTileProperties(const FTileData& TileData)
{
	TileMatchID = TileData.TileMatchId;
	MatchItemMesh->SetMaterial(0, TileData.TileMaterial);
}

void ATile::Reset() {
	Super::Reset();
	ToggleVisibility(true);
	bIsAvailable = true;
}

void ATile::ToggleVisibility(bool bShow) const {
	Close();
	TileMesh->SetVisibility(bShow);
}


void ATile::ShakeAndClose(){
	// Animate tile shake and close
	/*Close();
	bIsAvailable = true;
	BoardRef->bIsBusy = false;*/
	PlayShakeAnim();
}

void ATile::AfterShakeAnim() {
	Close();
	bIsAvailable = true;
	BoardRef->bIsBusy = false;
	BoardRef->CheckRemainTries(); // BUG: it causes the count decrease 2 time, each time for a tile, and also a hook for moves left, success and failure screens.
}


void ATile::Hide() {
	TileMesh->SetVisibility(false);
	MatchItemMesh->SetVisibility(false);
	BoardRef->bIsBusy = false;
}

// FActorDestroyedSignature ATile::OnTileDestroy() {
// 	
// }





