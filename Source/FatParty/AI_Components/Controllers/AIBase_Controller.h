
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIBase_Controller.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class FATPARTY_API AAIBase_Controller : public AAIController
{
	GENERATED_BODY()

public:
	AAIBase_Controller();

	UBlackboardComponent* GetBlackboardComponent() const;
	UBehaviorTreeComponent* GetBehaviorTreeComponent() const;

protected:

	UPROPERTY()
		UBehaviorTreeComponent* BehaviorTreeComponent = nullptr;
	UPROPERTY()
		UBlackboardComponent* BlackboardComponent = nullptr;


	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

};
