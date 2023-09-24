#include "LobbyGameMode.h"
#include "Blueprint/UserWidget.h"
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
		UE_LOG(LogTemp, Warning, TEXT("Three Playyers logged in"));

		//GetWorldTimerManager().SetTimer(TimerHandle, this, &ALobbyGameMode::StartPlayerGame, TimeToStart);
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ALobbyGameMode::StartPlayerGame, TimeToStart);

		// Aca llamar al widget de t
	}

	Controller = Cast<AThePlayerController>(NewPlayer);

		
	// Tenemos que crear el widget

	/*ConstructorHelpers::FClassFinder<UUserWidget>LoadingBP_Class(TEXT("/Game/Blueprints/UI/WBP_Loading"));
	if(!ensure(LoadingBP_Class.Class!=nullptr)) return;

	LoadingClass = LoadingBP_Class.Class;
	LoadingWidget = CreateWidget<UUserWidget>(this, LoadingClass);
	*/

	//LoadingWidget = CreateWidget<UUserWidget>(this, LoadingClass);
	//LoadingWidget = CreateWidget<UUserWidget>(Get, WBP_Loading);
	//LoadingWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--PlayersLoggedIn;
}

void ALobbyGameMode::StartPlayerGame()
{
	//GameInstance->StartPlayerGame();
	if (Controller)
	{
		// Call client's RPC
		Controller->OpenWidget();
	}

	GameInstance = Cast<UFatPartyGameInstance>(GetGameInstance());
	if(GameInstance == nullptr) return;

	UWorld* World = GetWorld();
	if(!ensure(World != nullptr)) return;
	GameInstance->SessionInterface->StartSession(NAME_GameSession);  // esto tal vez no debe ir ahi


	GameInstance->LoadingWBP();
	// Si aca tienes el session interface puedes decirle que empiece la session y que no permita mas ingresos.
	// se tiene que obtener la session.
	
	//NetworkError->AddToViewport();
	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/Dungeon_01?listen");
	
	//World->ServerTravel("/Game/Maps/LevelLoading?listen");

}
