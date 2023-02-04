#include "BasePowerUp.h"
#include "Components/SphereComponent.h"

ABasePowerUp::ABasePowerUp()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = SphereCollision;

	SphereCollision->InitSphereRadius(70.f);

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object Mesh"));
	ObjectMesh->SetupAttachment(SphereCollision);

	
}
