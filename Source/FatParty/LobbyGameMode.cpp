#include "LobbyGameMode.h"
#include "FatPartyCharacter.h"
#include "FatPartyGameInstance.h"
#include "FatPartyPlayerState.h"
#include "Controllers/ThePlayerController.h"
#include "Kismet/GameplayStatics.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	GameInstance = Cast<UFatPartyGameInstance>(GetGameInstance());
	if(GameInstance == nullptr) return;

	++PlayersLoggedIn;
	if(PlayersLoggedIn >= 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Playyers logged in"));

		GetWorldTimerManager().SetTimer(TimerHandle, this, &ALobbyGameMode::StartPlayerGame, TimeToStart);
	}

	Controller = Cast<AThePlayerController>(NewPlayer);

}

void ALobbyGameMode::HandlePlayerTransitionToGame()
{
    
    if (!GameInstance)
    {
        // Handle the case where the game instance is not of the expected type
        return;
    }

	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
	    PlayerController = Cast<APlayerController>(*It);

	    if (PlayerController)
	    {
	        AFatPartyPlayerState* CustomPlayerState = Cast<AFatPartyPlayerState>(PlayerController->PlayerState);
	        if (CustomPlayerState)
	        {
				GameInstance->SetSelectedPawnClass(CustomPlayerState->BP_GetUniqueId(), CustomPlayerState->SelectedPawnClass);
	        	//UE_LOG(LogTemp, Error, TEXT("PlayerController: %s, SelectedPawnClass: %p"),*PlayerController->GetName(), *CustomPlayerState->SelectedPawnClass->GetName());
	        }
	        else
	        {
	            // Handle the case where the player state is not of the expected type
	        }
	    }
	    else
	    {
	        // Handle the case where the controller is not of the expected type
	    }
	}
}


void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--PlayersLoggedIn;
}

void ALobbyGameMode::StartPlayerGame()
{
	if (Controller)
	{
		// Call client's RPC
		Controller->OpenWidget();
	}

	GameInstance = Cast<UFatPartyGameInstance>(GetGameInstance());
	if(GameInstance == nullptr) return;

	UWorld* TheWorld = GetWorld();
	if(!ensure(TheWorld != nullptr)) return;
	//GameInstance->SessionInterface->StartSession(NAME_GameSession);

	//GameInstance->LoadingWBP();
	
	bUseSeamlessTravel = true;
	TheWorld->ServerTravel("/Game/Maps/Dungeon_01?listen");
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Dungeon_01"));
}
