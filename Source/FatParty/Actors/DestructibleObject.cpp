#include "DestructibleObject.h"
#include "FatParty/FatPartyEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "PowerUps/BasePowerUp.h"

ADestructibleObject::ADestructibleObject()
{
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object Mesh"));
	RootComponent = ObjectMesh;
}



void ADestructibleObject::BeginPlay()
{
	Super::BeginPlay();

	ObjectMesh->OnComponentHit.AddDynamic(this, &ADestructibleObject::HitMesh);

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void ADestructibleObject::HitMesh(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{	
	if (OtherActor->IsA<AFatPartyEnemy>() && this->ActorHasTag("Grabbed") == false && this->GetVelocity().Z != 0)  
	{
		AController* MyOwnerInstigator = this->GetInstigatorController();
		UClass* DamageTypeClass = UDamageType::StaticClass();
		UGameplayStatics::ApplyDamage(OtherActor, DamageToEnemy, MyOwnerInstigator, this, DamageTypeClass);
		UGameplayStatics::ApplyDamage(this, DamageToSelf, MyOwnerInstigator, OtherActor, DamageTypeClass);
	}
}

void ADestructibleObject::HandleDestruction_Implementation()
{
	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}

	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	if(ObjectToDrop != nullptr)
	{
		
		SpawnDropObject();
	}

	Destroy();
}

void ADestructibleObject::SetObjectToDrop(ABasePowerUp* PickedItem)
{
	ObjectToDrop = PickedItem;
}

void ADestructibleObject::SpawnDropObject()
{
	if (ObjectToDrop != nullptr)
	{
		FVector PositionToSpawn = this->GetActorLocation() + FVector(0, 0, ZOffset);
		GenericBuildingClass = ObjectToDrop->GetClass();
		ABasePowerUp* BuildingToProspect = GetWorld()->SpawnActor<ABasePowerUp>(GenericBuildingClass, PositionToSpawn, this->GetActorRotation());

		if (BuildingToProspect == nullptr) return;

		BuildingToProspect->SetOwner(this);
	}
	
}




	
