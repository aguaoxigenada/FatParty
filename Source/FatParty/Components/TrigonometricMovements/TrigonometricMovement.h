#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TrigonometricMovement.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FATPARTY_API UTrigonometricMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTrigonometricMovement();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trignometric Values")
	FVector MaxMovementDisance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trignometric Values")
	FVector Frequency;

	AActor* Parent = GetOwner();
	float RunnningTime;

	UPROPERTY(VisibleAnywhere, Category = "Trignometric Values")
	FVector StartPosition;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
