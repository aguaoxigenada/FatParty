#include "ThePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "FatParty/Characters/FatPartyCharacter.h"
#include "FatParty/GameInstance/FatPartyGameInstance.h"
#include "FatParty/PlayerState/FatPartyPlayerState.h"
#include "FatParty/GameModes/GamplayGameMode.h"
#include "FatParty/GameModes/LobbyGameMode.h"
#include "FatParty/GameModes/FatPartyGameMode.h"
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

void AThePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    DOREPLIFETIME(AThePlayerController, MyPawnClass);
}



void AThePlayerController::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();
}


void AThePlayerController::RestartPlayer()
{
	AGameModeBase* GameMode = Cast<AGameModeBase>(GetWorld()->GetAuthGameMode());
    if(GameMode)
    {
	    GameMode->RestartPlayer(this);
    }
}

void AThePlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
    bShowMouseCursor = bPlayerEnabled;
}

UClass* AThePlayerController::GetPlayerPawnClass()
{
    return MyPawnClass; 
}

void AThePlayerController::DeterminePawnClass_Implementation()
{
    if (IsLocalController())
    {
        int random = FMath::RandRange(0, 1);
        if (random == 0)
        {
            ServerSetPawn(PawnToUseA);
            return;
        }

        ServerSetPawn(PawnToUseB);
    }
}

void AThePlayerController::HandlePlayerControllers_Implementation() 
{
    // Iterate over all player controllers in the world
    if (IsLocalController()) 
    {
    	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
	        PlayerController = Cast<AThePlayerController>(*It);
			
		    if (PlayerController)
		    {
	    		StartToSpawnInLevel(PlayerController);
	        }
	        else
		    {
		        // Handle the case where the controller is not of the expected type
		    }
	    }
    }

  
}

void AThePlayerController::StartToSpawnInLevel_Implementation(AThePlayerController* Controller)
{
    AGamplayGameMode* GameplayMode = Cast<AGamplayGameMode>(GetWorld()->GetAuthGameMode());
	GameplayMode->SpawnPlayerPawn(Controller);
}


bool AThePlayerController::ServerSetPawn_Validate(TSubclassOf<AFatPartyCharacter> InPawnClass)
{
    return true;
}


void AThePlayerController::ServerSetPawn_Implementation(TSubclassOf<AFatPartyCharacter> InPawnClass)
{

	MyPawnClass = InPawnClass;

    if(PlayerState)
    {
        AFatPartyPlayerState* ThePlayerState = GetPlayerState<AFatPartyPlayerState>();
        
        if (ThePlayerState)
		{
            ThePlayerState->SetSelectedPawnClass(InPawnClass);
			ALobbyGameMode* Lobby =  Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
			Lobby->HandlePlayerTransitionToGame();
        }
    }

	RestartPlayer();

}


void AThePlayerController::OpenWidget_Implementation()
{
    
	// Es la copia inicial del loading WBP
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

    if (Controller && /*ThePawnClass*/ MyPawnClass)
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
           // UE_LOG(LogTemp, Warning, TEXT("Spawm Point Obtained"));
            FActorSpawnParameters SpawnParams;
            SpawnParams.Instigator = nullptr;
            SpawnParams.Owner = Controller;
            APawn* NewPawn = GetWorld()->SpawnActor<APawn>(MyPawnClass, PlayerStartPoint->GetActorLocation(), PlayerStartPoint->GetActorRotation(), SpawnParams);
            
            // Possess the new pawn
            if (NewPawn)
            {
                PlayerRespawned(true);
                Controller->Possess(NewPawn);
            }
        }
    }

 
}

void AThePlayerController::SpawnPlayer(TSubclassOf<AFatPartyCharacter> ChoosenPawn)
{
    if (ChoosenPawn)
    {
        MyPawnClass = ChoosenPawn;
        RestartPlayer();
    }
}


void AThePlayerController::ServerRespawnPlayer_Implementation(AController* Controller)
{
    RespawnPlayer(Controller);
}


void AThePlayerController::SendToNextLevelClientCall()
{
  
    SendToNextLevel();
    OpenWidget();
	GetCharacter()->DisableInput(this);
}

void AThePlayerController::SendToNextLevel_Implementation()
{
    AFatPartyGameMode* PlayerGameMode = Cast<AFatPartyGameMode>(GetWorld()->GetAuthGameMode());
    PlayerGameMode->SendToNextLevel();
   
}

void AThePlayerController::OpenWidgetFromServer_Implementation()
{
    // Es la copia inicial del loading WBP
	UFatPartyGameInstance* GameInstance =  Cast<UFatPartyGameInstance>(GetGameInstance());

	LoadingWBPClass = GameInstance->LoadingClass;
    LoadingWBP = CreateWidget<UUserWidget>(this, LoadingWBPClass);

	if(!ensure(LoadingWBP !=nullptr)) return;

    LoadingWBP->AddToViewport();
    OpenWidget();
}




