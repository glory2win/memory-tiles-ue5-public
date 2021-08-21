// Fill out your copyright notice in the Description page of Project Settings.


#include "MemoryTilesGameMode.h"
#include "Board.h"

AMemoryTilesGameMode::AMemoryTilesGameMode() {
	PrimaryActorTick.bCanEverTick = false;
}

void AMemoryTilesGameMode::BeginPlay() {

	if (!BoardPrefab) return;
	
	auto* World = GetWorld();
	if(!World) return;

	FActorSpawnParameters Parameters;
	Parameters.Owner = this;

	BoardRef = World->SpawnActor<ABoard>(BoardPrefab, FVector(-130.f, -130.f, 0.f), FRotator::ZeroRotator, Parameters);		
	
}

void AMemoryTilesGameMode::RestartGame() const
{
	if (!BoardRef) return;

	BoardRef->ShuffleBoard();
}


