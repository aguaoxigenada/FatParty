#include "EndTriggerComponent.h"
#include "FatParty/FatPartyGameInstance.h"
#include "FatParty/UI/MenuSystem/InGameMenu.h"

UEndTriggerComponent::UEndTriggerComponent()
{
	 PrimaryComponentTick.bCanEverTick = true;
}

void UEndTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if(!bGameEnded)
    {
    	AActor* Actor = GetAcceptableActor();

		if (Actor != nullptr)
	    {
	        UPrimitiveComponent *Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
	        if (Component != nullptr)
	        {   
	            Component->SetSimulatePhysics(false);
	            Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

	            WonGame();
	            bGameEnded = true;
	        }
	    }  
    }
}

void UEndTriggerComponent::WonGame()
{
	UFatPartyGameInstance* GameInstance = Cast<UFatPartyGameInstance>(GetWorld()->GetGameInstance());
    if(GameInstance == nullptr) return;

    GameInstance->LoadInGameMenu();
    UInGameMenu* InGameMenu = Cast<UInGameMenu>(GameInstance->GetInGameMenu());
        
    InGameMenu->EndLevelScreen(true);  
}



