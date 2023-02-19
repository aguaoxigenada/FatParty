#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "PlatformTrigger.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FATPARTY_API UPlatformTrigger : public UBoxComponent
{
	GENERATED_BODY()

public:	
	
	UPlatformTrigger();  
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditInstanceOnly)
		FName AcceptableActorTag;

	virtual void BeginPlay() override;
	AActor* GetAcceptableActor() const;


private:
	UPROPERTY(EditInstanceOnly)
		TWeakObjectPtr<AActor> ChoosenPlatform;

	bool CheckIfActorIsPlayer(AActor* Player);
	void SetPlatform(AActor* ThePlatform);
	bool bIsPlayer;
	bool CheckedPlayer;

	UPlatformMovement* Platform;	

};
