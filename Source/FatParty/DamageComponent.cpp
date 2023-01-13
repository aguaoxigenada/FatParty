#include "DamageComponent.h"

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

}