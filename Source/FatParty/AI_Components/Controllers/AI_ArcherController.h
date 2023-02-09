// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIModule/Classes/Perception/AIPerceptionTypes.h"
#include "AIBase_Controller.h"
#include "AI_ArcherController.generated.h"

class UAISenseConfig_Sight;

UCLASS()
class FATPARTY_API AAI_ArcherController : public AAIBase_Controller
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = AI)
		UAISenseConfig_Sight* SightConfig;
	AAI_ArcherController();

protected:
	virtual void BeginPlay() override;

private:

	UFUNCTION()
		void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void OnPlayerLostFromSight();
	void OnPlayerHasBeenDetected();


	TWeakObjectPtr<AActor> TargetActor = nullptr;

	FTimerHandle PlayerLostFromSight;
	FTimerHandle PlayerHasBeenDetected;
	
};
