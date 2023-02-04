#include "SpeedPowerUp.h"
#include "Components/SphereComponent.h"
#include "FatParty/FatPartyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ASpeedPowerUp::ASpeedPowerUp()
{

}

void ASpeedPowerUp::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ASpeedPowerUp::OnSphereOverlap);
}

void ASpeedPowerUp::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AFatPartyCharacter>())
	{
		UActorComponent* Comp = OtherActor->GetComponentByClass(UCharacterMovementComponent::StaticClass());
		UCharacterMovementComponent* PlayerMovementComp = Cast<UCharacterMovementComponent>(Comp);
		PlayerMovementComp->MaxWalkSpeed = 1000;

		AFatPartyCharacter* Player = Cast<AFatPartyCharacter>(OtherActor);
		Player->StartExtraSpeedTimer();
		Destroy();
	}
}

	
