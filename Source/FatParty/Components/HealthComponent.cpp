#include "HealthComponent.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FatParty/GameMode/FatPartyGameMode.h"
#include "FatParty/UI/HudWidget.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	// OnTakeAnyDamage es un delegate que viene por defecto en el componente UActorComponent
	// Es una característica que forma parte de la clase Actor, lo que significa que cada clase
	// de actor admite de forma nativa infligir y recibir daño.

	// Este evento se activa desde el projectil.  Linea 59.
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
	
	FatPartyGameMode = Cast<AFatPartyGameMode>(UGameplayStatics::GetGameMode(this));

}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UHealthComponent::GetHealth()
{
	return Health;
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser)
{
	/* Todos estos valores vienen desde el Projectil : */

	// DamagedActor: El actor al que le estás haciendo daño.
	// Damage:  Cantidad de daño
	// Instigator: Representa el peón/personaje responsable de cualquier acción que realice el actor engendrado, como infligir daño
	// DamageCauser: El actor que realmente infligió este daño (por ejemplo, un proyectil o una granada).
	// UDamageType: Un objeto que puede influir en los cálculos de daño.

	if(Damage <= 0.f) return;

	Health -= Damage;

	OnPlayerDamaged.Broadcast();

	if(Health <= 0 && FatPartyGameMode)
	{
		FatPartyGameMode->ActorDied(DamagedActor);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Health: %f of %s"), Health, *DamagedActor->GetName());
}

