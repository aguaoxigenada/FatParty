#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Thrower.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FATPARTY_API UThrower : public UActorComponent
{
	GENERATED_BODY()

public:	

	UThrower();


	UFUNCTION(Server, Unreliable)
		void Server_Throw();
	UFUNCTION(NetMulticast, Unreliable)
		void Multicast_Throw();
	UFUNCTION(BlueprintCallable)
		void Throw();
	UPROPERTY(EditAnywhere)
		float ThrowForce = 5000;

protected:

	virtual void BeginPlay() override;
};
