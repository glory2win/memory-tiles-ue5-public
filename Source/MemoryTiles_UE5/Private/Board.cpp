// Fill out your copyright notice in the Description page of Project Settings.


#include "Board.h"


UBlockGrid::UBlockGrid() = default;


void UBlockGrid::Init(const uint32 SizeOfGrid)
{
	Size = SizeOfGrid;
	TileArray.Empty(Size * Size);
}

ATile* UBlockGrid::GetTileAt(const uint32 Row, const uint32 Column) const
{
	return TileArray[Row * Size + Column];
}

void UBlockGrid::Add(ATile* Tile)
{
	TileArray.Add(Tile);
}


// Sets default values
ABoard::ABoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UDataTable> TileDataTableObject(TEXT("DataTable'/Game/Data/TileDataTable.TileDataTable'"));
	if (TileDataTableObject.Succeeded()) {
		TileDataTable = TileDataTableObject.Object;
	}	
}

#pragma optimize("", off)

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();
	CreateBoard();

	NumberOfTries = MaxNumOfTries;
	RemainTiles = Size * Size;
	bIsBusy = false;

	PlayerController = Cast< AMemoryTilesPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController) {
		PlayerController->UpdateRemainMovesText(NumberOfTries);
	}
}



void ABoard::CreateBoard() {

	if (!TilePrefab) return;
	auto* world = GetWorld();
	if(!world) return;

	UE_LOG(LogTemp, Warning, TEXT("Creating board.."));

	Grid = NewObject<UBlockGrid>();
	Grid->Init(this->Size); // Board size is 3x3 usually.


	// Create board and store indices
	/**Total Grid Size = 2 * Size of Data Table rows + 1 */
	//TODO: Later replace this Size with the above
	for (uint8 i = 0; i < Size; i++) {
		for (uint8 j = 0; j < Size; j++) {

			const float XOffSet = i * TileSpacing;
			const float YOffSet = j * TileSpacing;
			const FVector TileLocation = FVector(XOffSet, YOffSet, 0.0f) + GetActorLocation();

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = this;

			ATile* NewTile = GetWorld()->SpawnActor<ATile>(
				TilePrefab,
				TileLocation,
				FRotator::ZeroRotator,
				SpawnParameters);

			NewTile->BoardRef = this;
			NewTile->AttachToActor(NewTile->BoardRef, FAttachmentTransformRules::KeepWorldTransform);
			
			Grid->Add(NewTile);

			UE_LOG(LogTemp, Warning, TEXT("Block location: %s"), *NewTile->GetActorLocation().ToString());
			
		}
	}

	ShuffleBoard();
	

	UE_LOG(LogTemp, Warning, TEXT("Board has created!"));	

}

#pragma optimize("", off)
void ABoard::ShuffleBoard()
{
	if (!TileDataTable) return;
	/** Iterate through the TArray with indices, Randomly select index and get the tile with that index, update the tile's graphic and id  */
	auto const TilesCount = Grid->GetGridLength(); // Grid size is based on how many tile types * 2 + 1 block for red blocker block. (2*d) + 1
	TArray<int32> IndicesArray;
	for (size_t i = 0; i < TilesCount; ++i)
	{
		this->Reset(); // Reset the current id.
		Grid->GetTileAtIndex(i)->Reset();
		IndicesArray.Add(i);
	}

	static const FString ContextString(TEXT("Tile Data Table Context"));
	
	static const TArray<FName> RowNameArray = TileDataTable->GetRowNames();	

	// Pick a random index to place red blocker tile 
	auto RandIndex = FMath::RandRange(0, IndicesArray.Num() - 1); //Note: Upper limit is included in RandRange()
	auto RandTileIndex = IndicesArray[RandIndex];
	auto RandTile = Grid->GetTileAtIndex(RandTileIndex);
	if (RandTile) {
		RandTile->SetAsBlockerTile();
		IndicesArray.RemoveAt(RandIndex);
	}

	int32 RowIndex = 0;
	while (IndicesArray.Num() != 0) {

		for (uint8 j = 0; j < 2; ++j) // 2 is match count for the tile, default is 2.
		{
			RandIndex = FMath::RandRange(0, IndicesArray.Num() - 1);
			RandTileIndex = IndicesArray[RandIndex];
			RandTile = Grid->GetTileAtIndex(RandTileIndex);
			if (RandTile != nullptr)
			{
				FTileData* TileData = TileDataTable->FindRow<FTileData>(RowNameArray[RowIndex], ContextString, true);
				RandTile->SetTileProperties(*TileData);				
				IndicesArray.RemoveAt(RandIndex);
				UE_LOG(LogTemp, Warning, TEXT("%s tile has updated and %d index has removed from array!"), *RandTile->GetName(), RandIndex);
			}
		}
		RowIndex++;
	}

	bIsFreshMatch = true;

	UE_LOG(LogTemp, Warning, TEXT("The board has been shuffled."));
}

void ABoard::CheckRemainTries() {
	--NumberOfTries;
	if (NumberOfTries <= 0) NumberOfTries = 0;

	if (PlayerController) {
		PlayerController->UpdateRemainMovesText(NumberOfTries);
		if(NumberOfTries == 0) {
			PlayerController->ShowGameFailHUD();
		}
	}	
}

void ABoard::CheckRemainTiles() {
	--RemainTiles;
	if (RemainTiles <= 0) {
		RemainTiles = 0;

		if (PlayerController) {
			PlayerController->ShowGameSuccessHUD();
		}
	}
}


void ABoard::Reset() {
	Super::Reset();
}

void ABoard::AddToSelectionArray(ATile* SelectedTile) {
	if(bIsFreshMatch) {
		TotalMatchCount = 2;
		SelectedTilesArray.Empty(); // Empty() can also call destructor.
		bIsFreshMatch = false;
	}
	SelectedTilesArray.Emplace(SelectedTile);
	--TotalMatchCount;
	if(TotalMatchCount <= 0) {
		CheckBoardForMatch();
	}
}

void ABoard::CheckBoardForMatch() {
	if (SelectedTilesArray.IsEmpty()) return;

	UE_LOG(LogTemp, Warning, TEXT("Checking board for match..."));

	bIsBusy = true; // Avoid clicking on other tiles while showing match results.

	bool bMatch = true;
	const auto FirstTileId = SelectedTilesArray[0]->GetTileMatchID();

	for(const auto Tile: SelectedTilesArray) {
		if( Tile->GetTileMatchID() != FirstTileId) {
			bMatch = false;
			break;
		}
	}
	
	for (const auto Tile : SelectedTilesArray) { // TODO: Try to move the logic to single for loop.
		if (bMatch) {
			Tile->OnMatchSuccess(); // Hide the tiles. Play success sound if any.
		}
		else {
			Tile->OnMatchFail(); // Shake the tile and close. Play failure sound 
		}
	}

	bIsFreshMatch = true;
}


#pragma optimize("", on)




