// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_Controller.h"

#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "AIModule/Classes/Perception/AIPerceptionSystem.h"
#include "AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "FatParty/AI_Components/AI_BossCharacter.h"
#include "FatParty/Components/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


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
	ChangeBossState(1);
	PerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ABoss_Controller::OnTargetPerceptionUpdated);
	
	if (AAI_BossCharacter* BossCharacter = Cast<AAI_BossCharacter>(this->GetPawn()))
	{;
	BossCharacter->OnTakeAnyDamage.AddUniqueDynamic(this,&ABoss_Controller::OnTakeAnyDamage);

	}
}

void ABoss_Controller::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (AAI_BossCharacter* BossCharacter = Cast<AAI_BossCharacter>(this->GetPawn()))
	{
		if (BossCharacter->GetHealthComponent()->GetHealth() >= 60)
		{
			ChangeBossState(1);
		}

		if (BossCharacter->GetHealthComponent()->GetHealth() < 60)
		{
			ChangeBossState(2);
		}
	}
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
			BehaviorTreeComponent->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>("TargetActor", TargetActor.Get());
			BehaviorTreeComponent->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("bPlayerDetected", true);
			BehaviorTreeComponent->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation", Lastposition);
			//ChangeBossState(1);
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

void ABoss_Controller::ChangeBossState(int stateValue)
{
	AAI_BossCharacter* BossCharacter = Cast<AAI_BossCharacter>(this->GetPawn());
	if(BossCharacter)
	{
		if(stateValue != BossCharacter->BossStateValue)
		{

			BossCharacter->BossStateValue = stateValue;

			BehaviorTreeComponent->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Int>("BossPhase", stateValue);

			if (stateValue == 1)
			{
				BossCharacter->GetCharacterMovement()->MaxWalkSpeed = 200.f;
			}
			else if (stateValue == 2)
			{
				BossCharacter->GetCharacterMovement()->MaxWalkSpeed = 400.f;
			}
		}

	}
	

}
