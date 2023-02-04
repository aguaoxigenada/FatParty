#include "FatPartyGameMode.h"

#include "FatParty/Actors/DestructibleObject.h"
#include "Kismet/GameplayStatics.h"
#include "FatParty/Actors/Tower.h"
#include "FatParty/Characters/KnightCharacter.h"
#include "FatParty/Controllers/ThePlayerController.h"

void AFatPartyGameMode::ActorDied(AActor *DeadActor)
{
    /* Condiciones de Victoria / Perder provisorias */

    // Se revisa desde el modo de juego si murio el personaje o si murieron todas las torretas

    if (DeadActor == FatPartyCharacter)
    {
        // HandleDestruction() se encuentra en FatPartyCharacter.
        FatPartyCharacter->HandleDestruction();
        if (PlayerController)
        {
            PlayerController->SetPlayerEnabledState(false);
        }
        //GameOver(false);

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

void AFatPartyGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Inicializa la configuracion que se quiere en el juego
    HandleGameStart();
    
}

void AFatPartyGameMode::HandleGameStart()
{
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
            PlayerController,                               // clase a utilizar
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

int32 AFatPartyGameMode::GetTargetTowerCount()
{
    // Calcula y retorna la cantidad de Torres que hay actualmente en el escenario.
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    return Towers.Num();
}
