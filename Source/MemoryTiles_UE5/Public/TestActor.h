// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "TestActor.generated.h"

UCLASS()
class MEMORYTILES_UE5_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle;

	UFUNCTION()
	void OnTimerFinished();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
