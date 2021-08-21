// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATestActor::OnTimerFinished, 2.f);

	if(GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "Timer has started!");
	}

	UE_LOG(LogTemp, Warning, TEXT("Timer has started!"));
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestActor::OnTimerFinished() {
	if(GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "Timer has finished!");
	}
	UE_LOG(LogTemp, Warning, TEXT("Timer has finished!"));
}


