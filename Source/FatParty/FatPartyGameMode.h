// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FatPartyGameMode.generated.h"


UCLASS()
class FATPARTY_API AFatPartyGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)  // No es necesario crearla en cpp cuando es un BlueprintImplementableEvent
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent) 
	void GameOver(bool bWonGame);


public:

	void ActorDied(AActor* DeadActor);

private:

	class ATank* Tank;
	class AThePlayerController* ToonTanksPlayerController;

	float StartDelay = 3.f;

	void HandleGameStart();	

	int32 TargetTowers = 0;
	int32 GetTargetTowerCount();
};
