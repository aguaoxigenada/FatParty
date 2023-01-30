#include "DestructibleObject.h"
#include "FatParty/FatPartyEnemy.h"
#include "Kismet/GameplayStatics.h"

ADestructibleObject::ADestructibleObject()
{
	//PrimaryActorTick.bCanEverTick = true;
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object Mesh"));
	RootComponent = ObjectMesh;
}

void ADestructibleObject::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	ObjectMesh->OnComponentHit.AddDynamic(this, &ADestructibleObject::HitMesh);

}

/*
 void ADestructibleObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Velocity = this->GetVelocity().Z;
	UE_LOG(LogTemp, Warning, TEXT("The float value is: %f"), Velocity);

}
*/

void ADestructibleObject::HitMesh(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{	
	if (OtherActor->IsA<AFatPartyEnemy>() && this->ActorHasTag("Grabbed") == false && this->GetVelocity().Z != 0)  
	{
		AController* MyOwnerInstigator = this->GetInstigatorController();
		UClass* DamageTypeClass = UDamageType::StaticClass();
		UGameplayStatics::ApplyDamage(OtherActor, DamageToEnemy, MyOwnerInstigator, this, DamageTypeClass);
		//HandleDestruction();  // que esto le baje vida
		UGameplayStatics::ApplyDamage(this, DamageToSelf, MyOwnerInstigator, OtherActor, DamageTypeClass);

	}
}

void ADestructibleObject::HandleDestruction()
{
	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}

	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	Destroy();
}



	
