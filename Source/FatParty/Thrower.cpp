// Fill out your copyright notice in the Description page of Project Settings.


#include "Thrower.h"
#include "Grabber.h"
#include "Tank.h"

// Sets default values for this component's properties
UThrower::UThrower()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UThrower::Throw()
{
	ATank* player = Cast<ATank>(GetOwner());
	AActor* ObjectToThrow = nullptr;
	if (player)
	{
		ObjectToThrow = player->ActorGrabbed;
		if (ObjectToThrow) 
		{
			player->TankGrabber->Release();
			UPrimitiveComponent* TargetObject = Cast<UPrimitiveComponent>(ObjectToThrow->GetRootComponent());
			//FVector Forward = player->GetActorForwardVector();
			FVector Forward = player->TurretMesh->GetForwardVector();
			TargetObject->AddImpulse(Forward * throwForce * TargetObject->GetMass());
		}
		

	}
}


// Called when the game starts
void UThrower::BeginPlay()
{
	Super::BeginPlay();

	ATank* player = Cast<ATank>(GetOwner());
	if (player)
	{
		player->SetTankThrower(this);

	}
	
}



