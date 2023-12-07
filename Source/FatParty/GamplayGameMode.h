// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/FatPartyGameMode.h"
#include "GamplayGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FATPARTY_API AGamplayGameMode : public AFatPartyGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "GamePlayGameMode")
	void HandlePlayerControllers();

	UFUNCTION(BlueprintCallable, Category = "Gameplay") // Deberia ser desde el server.?
    void SpawnPlayerPawn(AThePlayerController* NewPlayer);

	UFatPartyGameInstance* TheGameInstance;

private:
	AThePlayerController* PlayerController;
	AThePlayerController* NewPlayerController;
};
