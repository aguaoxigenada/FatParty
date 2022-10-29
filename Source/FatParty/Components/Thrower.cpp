#include "Thrower.h"
#include "Grabber.h"
#include "FatParty/Characters/KnightCharacter.h"

UThrower::UThrower()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UThrower::Throw()
{
	AKnightCharacter* Player = Cast<AKnightCharacter>(GetOwner());
	AActor* ObjectToThrow = nullptr;
	if (Player)
	{
		ObjectToThrow = Player->ActorGrabbed;
		if (ObjectToThrow) 
		{
			Player->TankGrabber->Release();
			UPrimitiveComponent* TargetObject = Cast<UPrimitiveComponent>(ObjectToThrow->GetRootComponent());
			//FVector Forward = player->GetActorForwardVector();
			FVector Forward = Player->TurretMesh->GetForwardVector();
			TargetObject->AddImpulse(Forward * ThrowForce * TargetObject->GetMass());
		}
		

	}
}

void UThrower::BeginPlay()
{
	Super::BeginPlay();

	AKnightCharacter* Player = Cast<AKnightCharacter>(GetOwner());
	if (Player)
	{
		Player->SetTankThrower(this);

	}
	
}



