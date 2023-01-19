// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_Controller.h"

#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "AIModule/Classes/Perception/AIPerceptionSystem.h"
#include "AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Object.h"




ABoss_Controller::ABoss_Controller() : Super()
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

void ABoss_Controller::BeginPlay()
{
	Super::BeginPlay();
	PerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ABoss_Controller::OnTargetPerceptionUpdated);
}

void ABoss_Controller::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus) == UAISense_Sight::StaticClass())
	{
		if (Stimulus.WasSuccessfullySensed())
		{

			if (PlayerLostFromSight.IsValid())
			{
				GetWorldTimerManager().ClearTimer(PlayerLostFromSight);
			}

			Lastposition = Stimulus.StimulusLocation;
			TargetActor = Actor;
		}
		else
		{

			if (!PlayerLostFromSight.IsValid())
			{
				GetWorldTimerManager().SetTimer(PlayerLostFromSight, this, &ABoss_Controller::OnPlayerLostFromSight, 3.f, true);
			}
		}
	}
}

void ABoss_Controller::OnPlayerLostFromSight()
{
	BehaviorTreeComponent->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("bPlayerDetected", false);
	if (PlayerLostFromSight.IsValid())
	{
		GetWorldTimerManager().ClearTimer(PlayerLostFromSight);
	}

	TargetActor = nullptr;
	BehaviorTreeComponent->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>("TargetActor", nullptr);

}

void ABoss_Controller::OnPlayerHasBeenDetected()
{
	if (PlayerHasBeenDetected.IsValid())
	{
		GetWorldTimerManager().ClearTimer(PlayerHasBeenDetected);
	}

	BehaviorTreeComponent->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>("TargetActor", TargetActor.Get());

}
