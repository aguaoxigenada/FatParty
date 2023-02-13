#pragma once

#include "CoreMinimal.h"
#include "TriggerComponent.h"
#include "EndTriggerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FATPARTY_API UEndTriggerComponent : public UTriggerComponent
{
	GENERATED_BODY()

public:
	UEndTriggerComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool bGameEnded = false;

	UFUNCTION(NetMulticast, Reliable)
		void WonGame();

};

