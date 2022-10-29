#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FATPARTY_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	UGrabber();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable) 
	void Grab();
	
	UFUNCTION(BlueprintCallable) 
	void Release();

	UPhysicsHandleComponent* GetPhysicsHandle() const;


private:
	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 400;

	UPROPERTY(EditAnywhere)
	float GrabRadius = 1; 

	UPROPERTY(EditAnywhere)
	float HoldDistance = 200; 

	bool GetGrabbableInReach(FHitResult &OutHitResult) const;
};
