// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Tank.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	ATank* player = Cast<ATank>(GetOwner());
	if (player)
	{
		player->SetTankGrabber(this);
	}

	// Chequeo por si se quiere comprobar que se tiene el mismo physicshandle que el blueprint
	// GetPhysicsHandle() busca el componente de Physics Handle y lo asigna a esta variable
	/*UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle != nullptr)
	{
		FString HandleName = PhysicsHandle->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Component name is: %s"), *HandleName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NoPointer!"));
	}
	*/
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();

	// Chequea si se tiene actualmente el componente de Physics Handle y si actualmente este tiene algo agarrado.
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;

		// Setea el objeto agarrado al lugar y posicion puestos
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabber::Grab()
{	
	/* Esta funcion se llama siempre que se apreta el mouse, esta en el EventGraph del Tank. */

	// Busca que el actor tenga un UPhysicsHandleComponent 
	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();
	
	// Chequeo de seguridad
	if (PhysicsHandle == nullptr)
	{
		return;
	}
	
	// 1. Llama a GetGrabbableInReach() para revisar que el objeto sea una gargola.
	// 2. De ser asi, prende la simulacion de fisicas de la gargola.
	// 3. Le asigna un tag "Grabbed" al actor (la gargola).
	// 4. Separa el RootComponent de este actor de cualquier componente de escena al que este conectado actualmente, pero le mantiene su Transform del mundo.
	// 5. Toma el componente en la ubicacion y rotacion en la que encuentra y lo retorna. 

	FHitResult HitResult;
	if (GetGrabbableInReach(HitResult))
	{
		UPrimitiveComponent *HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();

		ATank* player = Cast<ATank>(GetOwner());
		if (player)
		{
			player->ActorGrabbed = HitResult.GetActor();
		}
		AActor* ActorGrabbed = nullptr;
		ActorGrabbed = HitResult.GetActor();
		ActorGrabbed->Tags.Add("Grabbed");

		ActorGrabbed->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		// Como se tiene el physics handle, el objeto queda agarrado por esta funcion y luego en tick permite su movimiento.
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation());
	}
}

UPhysicsHandleComponent *UGrabber::GetPhysicsHandle() const
{
	// Busca que el actor tenga un UPhysicsHandleComponent y lo retorna.  Sino tira el error.
	UPhysicsHandleComponent *Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent."));
	}
	return Result;
}

void UGrabber::Release()
{
	UPhysicsHandleComponent *IsGrabbed = GetPhysicsHandle();
	UPrimitiveComponent* HasComponent = IsGrabbed->GetGrabbedComponent(); 
	
	if (IsGrabbed && HasComponent)  // primero el puntero de IsGrabed
	{
		ATank* player = Cast<ATank>(GetOwner());
		if (player)
		{
			player->ActorGrabbed = nullptr;
		}
		HasComponent->GetOwner()->Tags.Remove("Grabbed");
		IsGrabbed->ReleaseComponent();
	}
}

bool UGrabber::GetGrabbableInReach(FHitResult &OutHitResult) const
{
	// 1. Se crean dos vectores que se utilizan para mandar un rayo en el formato de SweepSingleByChannel
	// 2. El rayo en realidad esta basado en una forma, en este caso es una esfera.
	// 3. Los unicos objetos con los que puede colisionar son los del canal seteado: ECC_GameTraceChannel2
	// 4. Las gargolas se encuentran en ese canal.
	// 5. Se retorna el OutHitResult, que contiene la informacion de ese objeto

	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	
	// Para ver las lineas:
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2, 
		Sphere);
	
}
	
