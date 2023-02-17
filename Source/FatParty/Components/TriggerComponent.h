#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FATPARTY_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:	
	
	UTriggerComponent();  
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditInstanceOnly)
		FName AcceptableActorTag;

	virtual void BeginPlay() override;
	AActor* GetAcceptableActor() const;


private:
	UPROPERTY(EditInstanceOnly)
		TWeakObjectPtr<AActor> ChoosenMover;

	UPROPERTY(EditInstanceOnly)
		bool bKeepMoving = false;

	void SetMover(AActor* TheMover);

	UMover* Mover; 

	bool CheckIfActorIsPlayer(AActor* Player);
	bool bIsPlayer;
	bool CheckedPlayer;

	



	

};
