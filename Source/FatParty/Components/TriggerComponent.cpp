#include "TriggerComponent.h"
#include "Engine/EngineTypes.h"


UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
    Super::BeginPlay();
    SetMover(ChoosenMover.Get());
  
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (Mover == nullptr) return;
    
    AActor* Actor = GetAcceptableActor();

    if (Actor != nullptr)
    {
        CheckedPlayer = CheckIfActorIsPlayer(Actor);

        UPrimitiveComponent *Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if (Component != nullptr && !CheckedPlayer)
        {   
            // Le quita las fisicas al objeto que se coloco dentro del trigger.
            Component->SetSimulatePhysics(false);
            Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        }
        // Pone al Actor que se coloco en el trigger como parte del componente de trigger.
        
        Mover->SetShouldMove(true);
    }
    else if(bKeepMoving && Actor != nullptr)
    {
	     Mover->SetShouldMove(true);
    }
    else
    {
        Mover->SetShouldMove(false);
    }
}

bool UTriggerComponent::CheckIfActorIsPlayer(AActor* Player)
{
    if(Player == nullptr) return false;

    bIsPlayer = Player->ActorHasTag("ThePlayer");
    return bIsPlayer;
}

void UTriggerComponent::SetMover(AActor* TheMover)
{
    // Mover viene desde el include en TriggerComponent.h
    // Esta funcion se utiliza en Blueprint, SecretWall la tiene.

    if (TheMover == nullptr) return;

    TArray<UActorComponent*> Components;

	TheMover->GetComponents(Components);

    for(UActorComponent *Comp : Components)
	{
		UMover* thisComp = Cast<UMover>(Comp);
		if( thisComp )
		{
			//This is the mover component
            Mover = thisComp;
		}
	}
}

AActor *UTriggerComponent::GetAcceptableActor() const
{
    TArray<AActor *> Actors;
    
    // Llena el Array de Actors con todos los actores que hacen overlap.
    GetOverlappingActors(Actors);

    for (AActor *Actor : Actors) 
    {
        // A cada uno de los actores, revisa primero si el Tag es el "Aceptable"
        // Luego al Actor agarrado le pone el Tag de Grabbed.

        bool bHasAcceptableTag = Actor->ActorHasTag(AcceptableActorTag);
        bool bIsGrabbed = Actor->ActorHasTag("Grabbed");

    	if (bHasAcceptableTag && !bIsGrabbed)
        {
            //UE_LOG(LogTemp, Warning, TEXT("Hello"));
            return Actor;
        }
    }
    return nullptr;
}


