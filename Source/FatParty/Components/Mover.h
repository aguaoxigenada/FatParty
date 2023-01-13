#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FATPARTY_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	

	UMover();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
		void SetShouldMove(bool ShouldMove);

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		FVector MoveOffset;

	UPROPERTY(EditAnywhere)
		float MoveTime = 4;

	bool ShouldMove = false;

	FVector OriginalLocation;	
	
};
