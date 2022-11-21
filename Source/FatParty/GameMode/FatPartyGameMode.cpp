#include "FatPartyGameMode.h"

#include "FatParty/Tank.h"
#include "Kismet/GameplayStatics.h"
#include "FatParty/Actors/Tower.h"
#include "FatParty/Characters/KnightCharacter.h"
#include "FatParty/Controllers/ThePlayerController.h"

void AFatPartyGameMode::ActorDied(AActor *DeadActor)
{
    /* Condiciones de Victoria / Perder provisorias */

    // Se revisa desde el modo de juego si murio el personaje o si murieron todas las torretas

    if (DeadActor == KnightCharacter)
    {
        // HandleDestruction() se encuentra en BasePawn.
        KnightCharacter->HandleDestruction();
        if (KnightPlayerController)
        {
            KnightPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);

    }
    else if (ATower *DestroyedTower = Cast<ATower>(DeadActor))
    {
        DestroyedTower->HandleDestruction();
        TargetTowers--;
        if(TargetTowers == 0)
        {
            GameOver(true);
        }
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
    // UE_LOG(LogTemp, Warning, TEXT("Amount of Towers: %d"), TargetTowers);
    
    // Se hace el cast para recibir un puntero de tipo ATank en vez de uno APawn. 
  //  KnightCharacter = Cast<AKnightCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    KnightPlayerController = Cast<AThePlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    // Llama al evento creado en el EventGraph
    StartGame();

    if (KnightPlayerController)
    {
        KnightPlayerController->SetPlayerEnabledState(false);

        // Se necesita para poder usar el FTimerDelegate
        FTimerHandle PlayerEnableTimerHandle;
        
        // El delegate permite crear un objeto, que puede anexar a una funcion directamente.
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
            KnightPlayerController,                               // clase a utilizar
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
