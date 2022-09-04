// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FatPartyGameMode.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	// OnTakeAnyDamage es un delegate que viene por defecto en el componente UActorComponent
	// Es una característica que forma parte de la clase Actor, lo que significa que cada clase
	// de actor admite de forma nativa infligir y recibir daño.

	// Este evento se activa desde el projectil.  Linea 59.
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
	
	ToonTanksGameMode = Cast<AFatPartyGameMode>(UGameplayStatics::GetGameMode(this));
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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
	
	if(Health <= 0 && ToonTanksGameMode)
	{
		ToonTanksGameMode->ActorDied(DamagedActor);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Health: %f of %s"), Health, *DamagedActor->GetName());
}

