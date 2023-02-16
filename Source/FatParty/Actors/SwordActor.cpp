#include "SwordActor.h"
#include "Components/BoxComponent.h"

ASwordActor::ASwordActor()
{
	//WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	//RootComponent = WeaponMesh;
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxCollision->SetupAttachment(WeaponMesh);
}

void ASwordActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ActualZ = GetActorTransform().GetLocation().Z;

	if(ActualZ <= ZVectorLimit)
	{
		OnDestroyedSword.Broadcast();
		
		Destroy();
	}
}
