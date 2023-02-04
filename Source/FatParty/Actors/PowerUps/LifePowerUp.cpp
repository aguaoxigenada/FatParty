#include "LifePowerUp.h"
#include "Components/SphereComponent.h"
#include "FatParty/FatPartyCharacter.h"
#include "FatParty/Components/HealthComponent.h"

ALifePowerUp::ALifePowerUp()
{

}

void ALifePowerUp::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ALifePowerUp::OnSphereOverlap);
}


void ALifePowerUp::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA<AFatPartyCharacter>())
	{
		TArray<UActorComponent*> Components;

		OtherActor->GetComponents(Components);

	    for(UActorComponent *Comp : Components)
		{
			UHealthComponent* HealthComponent = Cast<UHealthComponent>(Comp);
			if( HealthComponent )
			{
				HealthComponent->AddHealth(LifeToAdd);
			}
		}

		Destroy();
	}
}




	
