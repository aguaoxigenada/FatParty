#include "Thrower.h"
#include "Grabber.h"
#include "FatParty/Characters/KnightCharacter.h"

UThrower::UThrower()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UThrower::Server_Throw_Implementation()
{
	Multicast_Throw();
}

void UThrower::Multicast_Throw_Implementation()
{
	Throw();
}

void UThrower::Throw()
{
	AFatPartyCharacter* Player = Cast<AFatPartyCharacter>(GetOwner());
	AActor* ObjectToThrow = nullptr;
	if (Player)
	{
		ObjectToThrow = Player->ActorGrabbed;
		if (ObjectToThrow) 
		{
			Player->PlayerGrabber->Release();
			UPrimitiveComponent* TargetObject = Cast<UPrimitiveComponent>(ObjectToThrow->GetRootComponent());
			FVector Forward = Player->GetActorForwardVector();
			TargetObject->AddImpulse(Forward * ThrowForce * TargetObject->GetMass());
		}
	}
}

void UThrower::BeginPlay()
{
	Super::BeginPlay();

	AFatPartyCharacter* Player = Cast<AFatPartyCharacter>(GetOwner());
	if (Player)
	{
		Player->SetThrower(this);
	}
}



