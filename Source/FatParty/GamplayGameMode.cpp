#include "GamplayGameMode.h"
#include "FatParty/FatPartyCharacter.h"
#include "FatPartyGameInstance.h"
#include "FatPartyPlayerState.h"
#include "Controllers/ThePlayerController.h"
#include "Kismet/GameplayStatics.h"

void AGamplayGameMode::BeginPlay()
{
    Super::BeginPlay();
    TheGameInstance = Cast<UFatPartyGameInstance>(UGameplayStatics::GetGameInstance(this));
	//HandlePlayerControllers();
}

void AGamplayGameMode::HandlePlayerControllers() // Este nombre deberia de cambiar tambien podria guardar los player controllers y enviarlos de nuevo y a la shit...
{
    // Iterate over all player controllers in the world
    for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        // entra pero es null. me parece que ese esta ok...
        PlayerController = Cast<AThePlayerController>(*It);
		AFatPartyPlayerState* ThisPlayerState = PlayerController->GetPlayerState<AFatPartyPlayerState>();
		FString PlayerIDToRetrievesss = ThisPlayerState->GetPlayerID();
	    if (PlayerController)
	    {
	    	SpawnPlayerPawn(PlayerController);
        }
        else
	    {
	        // Handle the case where the controller is not of the expected type
	    }
    }
}

void AGamplayGameMode::SpawnPlayerPawn(AThePlayerController* NewPlayer)
{
    if(TheGameInstance)
    {
	    // Log information about the GameInstance
	    UE_LOG(LogTemp, Error, TEXT("GameInstance is valid. Name: %s"), *TheGameInstance->GetName());
    }

	if (!TheGameInstance)
    {
        // Handle the case where the game instance is not of the expected type
        return;
    }

    APlayerStart* PlayerStartPoint = Cast<APlayerStart>(ChoosePlayerStart_Implementation(NewPlayer));

	AFatPartyPlayerState* PlayerState = Cast<AFatPartyPlayerState>(NewPlayer->PlayerState);  

	if (PlayerState)
	{
	    const FUniqueNetIdRepl& PlayerIDToRetrieve = PlayerState->BP_GetUniqueId();
		TSubclassOf<AFatPartyCharacter> SelectedPawnClass = TheGameInstance->PlayerPawnClassMap.FindRef(PlayerIDToRetrieve);

		if(SelectedPawnClass)
		{
			APawn* NewPawn = GetWorld()->SpawnActor<APawn>(SelectedPawnClass, PlayerStartPoint->GetActorLocation(), PlayerStartPoint->GetActorRotation());
			if (NewPawn)
			{
				NewPlayer->Possess(NewPawn);
			}
			else
			{
				// Handle the case where spawning the pawn fails
			}   
		}
		else
		{
			    // Handle the case where the selected pawn class is not found
		}
	}

}