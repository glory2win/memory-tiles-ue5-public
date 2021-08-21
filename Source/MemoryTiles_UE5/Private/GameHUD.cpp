// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"

void UGameHUD::ShowGameSuccessScreen() {
	UE_LOG(LogTemp, Warning, TEXT("You won the game!"));
}

void UGameHUD::ShowGameFailScreen() {
	UE_LOG(LogTemp, Warning, TEXT("Out of moves, try again!!"));
}

/*
void UGameHUD::UpdateRemainMoves(int32 RemainMoves) {
	UE_LOG(LogTemp, Warning, TEXT("Remain moves: %d"), RemainMoves);
}
*/

