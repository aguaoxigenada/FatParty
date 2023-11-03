#include "ThePlayerController.h"
#include "FatParty/FatPartyCharacter.h"
#include "FatParty/FatPartyGameInstance.h"
#include "FatParty/LobbyGameMode.h"
#include "FatParty/GameMode/FatPartyGameMode.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


AThePlayerController::AThePlayerController()
{
    /* Initialize The Values */
    PawnToUseA = NULL;
    PawnToUseB = NULL;

    /* Make sure the PawnClass is Replicated */
    bReplicates = true;
}

void AThePlayerController::BeginPlay()
{
	Super::BeginPlay();
   
}

void AThePlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
    /*if (bPlayerEnabled)
    {
        GetCharacter()->EnableInput(this);
    }
    else
    {
        GetCharacter()->DisableInput(this);
    }*/
    // bShowMouseCursor viene de la clase PlayerController, sirve para habilitar / deshabilitar el mouse.
    bShowMouseCursor = bPlayerEnabled;
}

UClass* AThePlayerController::GetPlayerPawnClass()
{
    return MyPawnClass; 
}

// Replication
void AThePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    DOREPLIFETIME(AThePlayerController, MyPawnClass);
}

void AThePlayerController::DeterminePawnClass_Implementation()
{
    if (IsLocalController()) //Only Do This Locally (NOT Client-Only, since Server wants this too!)
    {
        
        int random = FMath::RandRange(0, 1);
        /* Use PawnA if the Text File tells us to */
        if (random == 0)
        {
            ServerSetPawn(PawnToUseA);
            return;
        }

        /* Otherwise, Use PawnB :) */
        ServerSetPawn(PawnToUseB);
        return;
    }
}


bool AThePlayerController::ServerSetPawn_Validate(TSubclassOf<AFatPartyCharacter> InPawnClass)
{
    return true;
}

void AThePlayerController::ServerSetPawn_Implementation(TSubclassOf<AFatPartyCharacter> InPawnClass)
{
    MyPawnClass = InPawnClass;

    /* Just in case we didn't get the PawnClass on the Server in time... */
   
    AGameModeBase* GameMode = Cast<AGameModeBase>(GetWorld()->GetAuthGameMode());
    if(GameMode)
    {
	    GameMode->RestartPlayer(this);
    }
}

void AThePlayerController::OpenWidget_Implementation()
{

	// Es la copia inicial del loading WBP (Ver si es o no necesaria la de blueprint)
	UFatPartyGameInstance* GameInstance =  Cast<UFatPartyGameInstance>(GetGameInstance());

	LoadingWBPClass = GameInstance->LoadingClass;
    LoadingWBP = CreateWidget<UUserWidget>(this, LoadingWBPClass);

	if(!ensure(LoadingWBP !=nullptr)) return;

    LoadingWBP->AddToViewport();
    
}


void AThePlayerController::RespawnPlayer(AController* Controller)
{
    AFatPartyGameMode* PlayerGameMode = Cast<AFatPartyGameMode>(GetWorld()->GetAuthGameMode());
    // Check if the Controller is valid and has a valid pawn

    if (Controller && ThePawnClass)
    {
        // Destroy the existing pawn
        APawn* ExistingPawn = Controller->GetPawn();

    	if (ExistingPawn)
        {
            ExistingPawn->Destroy();
        }

        // Spawn a new pawn at a chosen spawn point
        if (APlayerStart* PlayerStartPoint = Cast<APlayerStart>(PlayerGameMode->ChoosePlayerStart_Implementation(Controller)))
        {
            UE_LOG(LogTemp, Warning, TEXT("Spawm Point Obtained"));
            FActorSpawnParameters SpawnParams;
            SpawnParams.Instigator = nullptr;
            SpawnParams.Owner = Controller;
            APawn* NewPawn = GetWorld()->SpawnActor<APawn>(ThePawnClass, PlayerStartPoint->GetActorLocation(), PlayerStartPoint->GetActorRotation(), SpawnParams);
            
            // Possess the new pawn
            if (NewPawn)
            {
                PlayerRespawned(true);
                Controller->Possess(NewPawn);
            }
        }
    }

 
}


void AThePlayerController::ServerRespawnPlayer_Implementation(AController* Controller)
{
    RespawnPlayer(Controller);
}

void AThePlayerController::SendToNextLevelClientCall()
{
  
    SendToNextLevel();
    // Hay que bloquear al player client, quitar el hud e ir poniendo un loading WBP.
    // Tambien funcionaria para 

}

void AThePlayerController::SendToNextLevel_Implementation()
{
    AFatPartyGameMode* PlayerGameMode = Cast<AFatPartyGameMode>(GetWorld()->GetAuthGameMode());
    PlayerGameMode->TriggerNextLevel();
   
}



