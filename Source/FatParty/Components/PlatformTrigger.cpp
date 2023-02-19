#include "PlatformTrigger.h"
#include "PlatformMovement.h"
#include "Engine/EngineTypes.h"


UPlatformTrigger::UPlatformTrigger()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UPlatformTrigger::BeginPlay()
{
    Super::BeginPlay();
    SetPlatform(ChoosenPlatform.Get());
  
}

void UPlatformTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (Platform == nullptr) return;
    
    AActor* Actor = GetAcceptableActor();

    if (Actor != nullptr)
    {
        CheckedPlayer = CheckIfActorIsPlayer(Actor);

        UPrimitiveComponent *Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if (Component != nullptr && !CheckedPlayer)
        {   
            Component->SetSimulatePhysics(false);
            Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        }

    	Platform->SetShouldMove(true);
    }
  
    else
    {
        Platform->SetShouldMove(false);
    }
}

bool UPlatformTrigger::CheckIfActorIsPlayer(AActor* Player)
{
    if(Player == nullptr) return false;

    bIsPlayer = Player->ActorHasTag("ThePlayer");
    return bIsPlayer;
}

void UPlatformTrigger::SetPlatform(AActor* ThePlatform)
{
    if (ThePlatform == nullptr) return;

    TArray<UActorComponent*> Components;

	ThePlatform->GetComponents(Components);

    for(UActorComponent *Comp : Components)
	{
		UPlatformMovement* thisComp = Cast<UPlatformMovement>(Comp);
		if( thisComp )
		{
            Platform = thisComp;
		}
	}
}

AActor *UPlatformTrigger::GetAcceptableActor() const
{
    TArray<AActor *> Actors;
    
    GetOverlappingActors(Actors);

    for (AActor *Actor : Actors) 
    {
        bool bHasAcceptableTag = Actor->ActorHasTag(AcceptableActorTag);
        bool bIsGrabbed = Actor->ActorHasTag("Grabbed");

    	if (bHasAcceptableTag && !bIsGrabbed)
        {
            return Actor;
        }
    }
    return nullptr;
}


