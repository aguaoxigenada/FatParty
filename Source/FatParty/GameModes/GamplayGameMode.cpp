#include "GamplayGameMode.h"
#include "FatParty/Characters/FatPartyCharacter.h"
#include "FatParty/Controllers/ThePlayerController.h"
#include "FatParty/GameInstance/FatPartyGameInstance.h"
#include "FatParty/PlayerState/FatPartyPlayerState.h"
#include "Kismet/GameplayStatics.h"

void AGamplayGameMode::BeginPlay()
{
    Super::BeginPlay();
    TheGameInstance = Cast<UFatPartyGameInstance>(UGameplayStatics::GetGameInstance(this));
}

void AGamplayGameMode::SpawnPlayerPawn(AThePlayerController* NewPlayer)
{
    if(TheGameInstance)
    {
	    // Log information about the GameInstance
	    UE_LOG(LogTemp, Error, TEXT("GameInstance is valid. Name: %s"), *TheGameInstance->GetName());

		APlayerStart* PlayerStartPoint = Cast<APlayerStart>(ChoosePlayerStart_Implementation(NewPlayer));

		AFatPartyPlayerState* PlayerState = Cast<AFatPartyPlayerState>(NewPlayer->PlayerState);  

		if (PlayerState)
		{
			const FUniqueNetIdRepl& PlayerIDToRetrieve = PlayerState->BP_GetUniqueId();
			TSubclassOf<AFatPartyCharacter> SelectedPawnClass = TheGameInstance->PlayerPawnClassMap.FindRef(PlayerIDToRetrieve);

			if(SelectedPawnClass)
			{
				NewPlayer->SpawnPlayer(SelectedPawnClass);
				//
			}
		}
    }

	else
    {
        // Handle the case where the game instance is not of the expected type
    }

}