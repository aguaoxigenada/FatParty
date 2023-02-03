// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIModule/Classes/Perception/AIPerceptionTypes.h"
#include "FatParty/AI_Components/Controllers/AIBase_Controller.h"
#include "Boss_Controller.generated.h"

class UAISenseConfig_Sight;

UCLASS()
class FATPARTY_API ABoss_Controller : public AAIBase_Controller
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category = AI)
		UAISenseConfig_Sight* SightConfig;


	ABoss_Controller();

	

protected:
	virtual void BeginPlay() override;


public:
	UFUNCTION()
		void OnTakeAnyDamage(AActor*  DamagedActor , float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);


private:
	UFUNCTION()
		void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);



	void OnPlayerLostFromSight();
	void OnPlayerHasBeenDetected();
	void ChangeBossState(int stateValue);

	TWeakObjectPtr<AActor> TargetActor = nullptr;
	FVector Lastposition = FVector();

	FTimerHandle PlayerLostFromSight;
	FTimerHandle PlayerHasBeenDetected;
};


