#include "ThePlayerController.h"
#include "FatParty/FatPartyCharacter.h"
#include "FatParty/FatPartyGameInstance.h"
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

   // ConstructorHelpers::FClassFinder<UUserWidget>LoadingBP_Class(TEXT("/Game/Blueprints/UI/WBP_Loading"));
	//if(!ensure(LoadingBP_Class.Class!=nullptr)) return;
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

void AThePlayerController::BeginPlay()
{
	Super::BeginPlay();

    DeterminePawnClass();
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
    UFatPartyGameInstance* GameInstance =  Cast<UFatPartyGameInstance>(GetGameInstance());

	LoadingWBPClass = GameInstance->LoadingClass;
    LoadingWBP = CreateWidget<UUserWidget>(this, LoadingWBPClass);

	if(!ensure(LoadingWBP !=nullptr)) return;

    LoadingWBP->AddToViewport();
    
}

