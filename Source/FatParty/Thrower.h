// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Thrower.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FATPARTY_API UThrower : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UThrower();

	UFUNCTION(BlueprintCallable)
		void Throw();
	UPROPERTY(EditAnywhere)
		float throwForce = 5000;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

		
};
