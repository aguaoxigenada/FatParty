#include "LobbyGameMode.h"
#include "FatPartyGameInstance.h"
#include "Controllers/ThePlayerController.h"

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

	UWorld* World = GetWorld();
	if(!ensure(World != nullptr)) return;
	GameInstance->SessionInterface->StartSession(NAME_GameSession);

	GameInstance->LoadingWBP();
	
	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/Dungeon_01?listen");
}
