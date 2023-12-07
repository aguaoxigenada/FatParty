#include "FatPartyGameMode.h"

#include "FatParty/FatPartyCharacter.h"
#include "FatParty/Actors/DestructibleObject.h"
#include "Kismet/GameplayStatics.h"
#include "FatParty/Actors/Tower.h"
#include "FatParty/Controllers/ThePlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "FatParty/FatPartyGameInstance.h"
#include "GameFramework/GameStateBase.h"

AFatPartyGameMode::AFatPartyGameMode()
{
    PlayerControllerClass = AThePlayerController::StaticClass();
    PopulatePlayerStartArray();
   
}

void AFatPartyGameMode::BeginPlay()
{
    Super::BeginPlay();

	World = GetWorld();
	CurrentLevelName = World->GetFName();
}





void AFatPartyGameMode::ActorDied(AActor *DeadActor)
{
    /* Condiciones de Victoria / Perder provisorias */

    // Se revisa desde el modo de juego si murio el personaje o si murieron todas las torretas

    if (AFatPartyCharacter* DestroyedPlayer = Cast<AFatPartyCharacter>(DeadActor))
    {
        DestroyedPlayer->HandleDestruction();
        if (PlayerController)
        {
            PlayerController->SetPlayerEnabledState(false);
        }
    }

    else if (AFatPartyEnemy* DestroyedEnemy = Cast<AFatPartyEnemy>(DeadActor))
    {
        DestroyedEnemy->HandleDestruction();

    	/*TargetTowers--;
        if(TargetTowers == 0)
        {
            GameOver(true);
        }*/    
    }
    else if(ADestructibleObject* DestroyedObject = Cast<ADestructibleObject>(DeadActor))
    {
	    DestroyedObject->HandleDestruction();
    }
}


UClass* AFatPartyGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    /* Override Functionality to get Pawn from PlayerController */
    AThePlayerController* MyController = Cast<AThePlayerController>(InController);

    // Aca se puede hacer un if que consulte si estas en en el lobby haces esto sino pes te vas a la otra opcion.
    if (MyController)
    {
        return MyController->GetPlayerPawnClass();
    }

    /* If we don't get the right Controller, use the Default Pawn */
    return DefaultPawnClass;
}

/*
void AFatPartyGameMode::HandleGameStart()
{
    //Esta funcion no se usa, ver de borrarla.
    // Calcula y retorna la cantidad de torres actual en el escenario
    TargetTowers = GetTargetTowerCount();
    FatPartyCharacter = Cast<AFatPartyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    PlayerController = Cast<AThePlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    // Llama al evento creado en el EventGraph
    StartGame();

    if (PlayerController)
    {
        PlayerController->SetPlayerEnabledState(false);

        // Se necesita para poder usar el FTimerDelegate
        FTimerHandle PlayerEnableTimerHandle;
        
        // El delegate permite crear un objeto, que puede anexar a una funcion directamente.
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
            PlayerController,                                        // clase a utilizar
            &AThePlayerController::SetPlayerEnabledState,            // funcion de la clase
            true                                                     // parametro pasado a la clase
        );

        // Timer seteado con el tiempo del juego.
            GetWorldTimerManager().SetTimer(
                PlayerEnableTimerHandle, 
                PlayerEnableTimerDelegate,                           // Se llama despues del tiempo seteado en el StartDelay. 
                StartDelay, 
                false
        );
    }
}
*/

int32 AFatPartyGameMode::GetTargetTowerCount()
{
    // Calcula y retorna la cantidad de Torres que hay actualmente en el escenario.
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    return Towers.Num();
}

AActor* AFatPartyGameMode::ChoosePlayerStart_Implementation(AController* Player)
{

    if (PlayerStartPoints.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, PlayerStartPoints.Num() - 1);
        return PlayerStartPoints[RandomIndex];
    }

    return nullptr;
}


void AFatPartyGameMode::SendToNextLevel()
{

    if(HasAuthority())
    {
    	LoadWBPLoadingOnClient();
    }

	GameInstance = Cast<UFatPartyGameInstance>(GetGameInstance());
	if(GameInstance == nullptr) return;

    GameInstance->LoadingWBP();

	bUseSeamlessTravel = true;

	int LevelIndex = -1;

	// Map level of names to integers
	if (CurrentLevelName == "Dungeon_01") LevelIndex = 0;
	else if (CurrentLevelName == "Dungeon_02") LevelIndex = 1;
	else if (CurrentLevelName == "Dungeon_03") LevelIndex = 2;
	else if (CurrentLevelName == "Dungeon_04") LevelIndex = 3;

	switch (LevelIndex)
	{
	    case 0:
	        World->ServerTravel("/Game/Maps/Level_02/Dungeon_02");
	        break;

	    case 1:
	        World->ServerTravel("/Game/Maps/Level_03/Dungeon_03");
	        break;

	    case 2:
	        World->ServerTravel("/Game/Maps/Level_04/Dungeon_04");
	        break;

	    case 3:
	        World->ServerTravel("/Game/Maps/Level_05/Dungeon_05");
	        break;

	    default:
	        break;
	}
}

void AFatPartyGameMode::LoadWBPLoadingOnClient_Implementation()
{
  for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AThePlayerController* ThePlayerController = Cast<AThePlayerController>(*It);
  		if (ThePlayerController)
        {
            ThePlayerController->OpenWidget();
        }
    }
}


void AFatPartyGameMode::PopulatePlayerStartArray()
{
    // Find all PlayerStart actors in the current level
    TArray<AActor*> PlayerStartActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartActors);

    // Loop through the found PlayerStart actors and add them to the PlayerStartArray
    for (AActor* Actor : PlayerStartActors)
    {
        APlayerStart* PlayerStart = Cast<APlayerStart>(Actor);
        if (PlayerStart)
        {
            PlayerStartPoints.Add(PlayerStart);
        	//UE_LOG(LogTemp, Warning, TEXT("Element %d"), PlayerStart);
        }
    }
}
