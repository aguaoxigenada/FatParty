// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerComponent.h"
#include "Engine/EngineTypes.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    //  UE_LOG(LogTemp, Warning, TEXT("Constructing")); // ocurre primero
    // ...
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (Mover == nullptr)
    {
        return;
    }

    AActor *Actor = GetAcceptableActor();
    if (Actor != nullptr)
    {
        UPrimitiveComponent *Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if (Component != nullptr)
        {   
            // Le quita las fisicas al objeto que se coloco dentro del trigger.
            Component->SetSimulatePhysics(false);
        }
        // Pone al Actor que se coloco en el trigger como parte del componente de trigger.
        Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        Mover->SetShouldMove(true);
    }
    else
    {
        Mover->SetShouldMove(false);
    }
}

void UTriggerComponent::SetMover(UMover *NewMover)
{
    // Mover viene desde el include en TriggerComponent.h
    // Esta funcion se utiliza en Blueprint, SecretWall la tiene.

    Mover = NewMover;
}

AActor *UTriggerComponent::GetAcceptableActor() const
{
    TArray<AActor *> Actors;
    
    // Llena el Array de Actors con todos los actores que hacen overlap.
    GetOverlappingActors(Actors);

    // for all AActors in Actors do:   (es como el in en C#)
    for (AActor *Actor : Actors) 
    {
        // A cada uno de los actores, revisa primero si el Tag es el "Aceptable"
        // Luego al Actor agarrado le pone el Tag de Grabbed.

        bool HasAcceptableTag = Actor->ActorHasTag(AcceptableActorTag);
        bool IsGrabbed = Actor->ActorHasTag("Grabbed");

        if (HasAcceptableTag && !IsGrabbed)
        {
            return Actor;
        }
    }
    return nullptr;
}
