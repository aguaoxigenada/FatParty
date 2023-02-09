// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_ArcherController.h"

#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "AIModule/Classes/Perception/AIPerceptionSystem.h"
#include "AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "FatParty/FatPartyCharacter.h"
#include "FatParty/LongDistanceEnemies/LongDistanceEnemies.h"

AAI_ArcherController::AAI_ArcherController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SightConfig->SightRadius = 500.f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 20.f;
	SightConfig->PeripheralVisionAngleDegrees = 45.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());


}

void AAI_ArcherController::BeginPlay()
{
	Super::BeginPlay();
	PerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AAI_ArcherController::OnTargetPerceptionUpdated);

}

void AAI_ArcherController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus) == UAISense_Sight::StaticClass())
	{
		if (Stimulus.WasSuccessfullySensed())
		{

			if (PlayerLostFromSight.IsValid())
			{
				GetWorldTimerManager().ClearTimer(PlayerLostFromSight);
			}

			if (!PlayerHasBeenDetected.IsValid())
			{
				GetWorldTimerManager().SetTimer(PlayerHasBeenDetected, this, &AAI_ArcherController::OnPlayerHasBeenDetected, 1.f, true);

			}

			TargetActor = Actor;


		}
		else
		{

			if (!PlayerLostFromSight.IsValid())
			{
				GetWorldTimerManager().SetTimer(PlayerLostFromSight, this, &AAI_ArcherController::OnPlayerLostFromSight, 3.f, true);
			}
		
		}
	}
}

void AAI_ArcherController::OnPlayerLostFromSight()
{

	if (PlayerLostFromSight.IsValid())
	{
		GetWorldTimerManager().ClearTimer(PlayerLostFromSight);
	}

	TargetActor = nullptr;
	
}

void AAI_ArcherController::OnPlayerHasBeenDetected()
{

	if (PlayerHasBeenDetected.IsValid())
	{
		GetWorldTimerManager().ClearTimer(PlayerHasBeenDetected);
	}

	if (AFatPartyCharacter* PlayerCharacter = Cast<AFatPartyCharacter>(TargetActor.Get()))
	{
		TargetActor = PlayerCharacter;

		if(ALongDistanceEnemies* m_ControlledPawn = Cast<ALongDistanceEnemies>(GetPawn()))
		{
			m_ControlledPawn->FatPartyCharacter = PlayerCharacter;
		}
		
	}
}
