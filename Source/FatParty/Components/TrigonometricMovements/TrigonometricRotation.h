#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TrigonometricRotation.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FATPARTY_API UTrigonometricRotation : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTrigonometricRotation();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trignometric Values")
		FVector MaxAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trignometric Values")
		FVector Frequency;

	AActor* Parent = GetOwner();
	float RunnningTime;

	UPROPERTY(VisibleAnywhere, Category = "Trignometric Values")
		FRotator StartRotation;
protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
