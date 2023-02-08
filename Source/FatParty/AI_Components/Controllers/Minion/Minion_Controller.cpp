// Fill out your copyright notice in the Description page of Project Settings.


#include "Minion_Controller.h"

#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "AIModule/Classes/Perception/AIPerceptionSystem.h"
#include "AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Components/SphereComponent.h"
#include "FatParty/FatPartyCharacter.h"
#include "GameFramework/Character.h"


AMinion_Controller::AMinion_Controller()
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

void AMinion_Controller::BeginPlay()
{
	Super::BeginPlay();
	InitialPosition = GetPawn()->GetActorLocation();
	PerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AMinion_Controller::OnTargetPerceptionUpdated);
	BehaviorTreeComponent->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("InitialPosition", InitialPosition);


}

void AMinion_Controller::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus) == UAISense_Sight::StaticClass())
	{
		if (Stimulus.WasSuccessfullySensed())
		{

			if (PlayerLostFromSight.IsValid())
			{
				GetWorldTimerManager().ClearTimer(PlayerLostFromSight);
			}

			if(!PlayerHasBeenDetected.IsValid())
			{
				GetWorldTimerManager().SetTimer(PlayerHasBeenDetected, this, &AMinion_Controller::OnPlayerHasBeenDetected, 1.f, true);

			}

			Lastposition = Stimulus.StimulusLocation;
			TargetActor = Actor;

		

			
		}
		else
		{

			if (!PlayerLostFromSight.IsValid())
			{
				GetWorldTimerManager().SetTimer(PlayerLostFromSight, this, &AMinion_Controller::OnPlayerLostFromSight, 3.f, true);
			}
		}
	}
}

void AMinion_Controller::OnPlayerLostFromSight()
{
	BehaviorTreeComponent->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("bPlayerDetected", false);
	if (PlayerLostFromSight.IsValid())
	{
		GetWorldTimerManager().ClearTimer(PlayerLostFromSight);
	}

	TargetActor = nullptr;
	BehaviorTreeComponent->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>("TargetActor", nullptr);


}

void AMinion_Controller::OnPlayerHasBeenDetected()
{
	if (PlayerHasBeenDetected.IsValid())
	{
		GetWorldTimerManager().ClearTimer(PlayerHasBeenDetected);
	}

	if (AFatPartyCharacter* PlayerCharacter = Cast<AFatPartyCharacter>(TargetActor.Get()))
	{

		BehaviorTreeComponent->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>("TargetActor", TargetActor.Get());
		BehaviorTreeComponent->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("bPlayerDetected", true);
		BehaviorTreeComponent->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation", Lastposition);

	}

}


