#include "SwordActor.h"

ASwordActor::ASwordActor()
{
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

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
