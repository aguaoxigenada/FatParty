#include "AIBase_Controller.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FatParty/AI_Components/AI_CharacterBase.h"

AAIBase_Controller::AAIBase_Controller()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));

}

UBlackboardComponent* AAIBase_Controller::GetBlackboardComponent() const
{
	return BlackboardComponent;
}

UBehaviorTreeComponent* AAIBase_Controller::GetBehaviorTreeComponent() const
{
	return BehaviorTreeComponent;
}

void AAIBase_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AAI_CharacterBase* CharacterBase = Cast<AAI_CharacterBase>(InPawn);
	UBehaviorTree* CharacterBehaviorTree = CharacterBase ? CharacterBase->GetBehaviorTree() : nullptr;

	if (CharacterBehaviorTree)
	{
		BlackboardComponent->InitializeBlackboard(*CharacterBehaviorTree->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*CharacterBehaviorTree);
	}
}

void AAIBase_Controller::BeginPlay()
{
	Super::BeginPlay();
}
