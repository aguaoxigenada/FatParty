#include "DamageComponent.h"

#include "Kismet/GameplayStatics.h"

UDamageComponent::UDamageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Parent = GetOwner();
}

void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent*> Components;

	Parent->GetComponents(Components);

    for(UActorComponent *Comp : Components)
	{
		UStaticMeshComponent* ThisComp = Cast<UStaticMeshComponent>(Comp);
		if( ThisComp )
		{
            StaticMesh = ThisComp;
		}
	}

	StaticMesh->OnComponentHit.AddDynamic(this, &UDamageComponent::OnHit);
}

void UDamageComponent::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AController* MyOwnerInstigator = Parent->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != Parent)
	{
		// Esto genera el evento para OnTakeAnyDamage del HealthComponent
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, Parent, DamageTypeClass);

		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, Parent->GetActorLocation());
		}

		if(HitCameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}
	}
}