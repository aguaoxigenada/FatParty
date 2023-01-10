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
	// Called when the game starts
	virtual void BeginPlay() override;
	AActor* GetAcceptableActor() const;

	UPROPERTY(EditInstanceOnly)
		FName AcceptableActorTag;	
private:
	UMover* Mover; 

	bool CheckIfActorIsPlayer(AActor* Player);
	bool bIsPlayer;
	bool CheckedPlayer;

	UPROPERTY(EditInstanceOnly)
		TWeakObjectPtr<AActor> ChoosenMover;

	void SetMover(AActor* TheMover); 

};
