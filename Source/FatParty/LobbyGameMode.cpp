#include "LobbyGameMode.h"
#include "FatPartyGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++PlayersLoggedIn;
	if(PlayersLoggedIn >= 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Three Playyers logged in"));

		GetWorldTimerManager().SetTimer(TimerHandle, this, &ALobbyGameMode::StartPlayerGame, TimeToStart);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--PlayersLoggedIn;
}

void ALobbyGameMode::StartPlayerGame()
{
	UFatPartyGameInstance* GameInstance = Cast<UFatPartyGameInstance>(GetGameInstance());
	if(GameInstance == nullptr) return;

	UWorld* World = GetWorld();
	if(!ensure(World != nullptr)) return;

	bUseSeamlessTravel = true;
	GameInstance->SessionInterface->StartSession(NAME_GameSession);

	// Si aca tienes el session interface puedes decirle que empiece la session y que no permita mas ingresos.
	// se tiene que obtener la session.

	World->ServerTravel("/Game/Maps/Dungeon_01?listen");

}
