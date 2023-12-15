#include "FatPartyPlayerState.h"
#include "Net/UnrealNetwork.h"


void AFatPartyPlayerState::SetSelectedPawnClass(TSubclassOf<AFatPartyCharacter> NewSelectedClass)
{
    // Set the selected pawn class
    SelectedPawnClass = NewSelectedClass;
}

FString AFatPartyPlayerState::GenerateUUID()
{
    FGuid UUID = FGuid::NewGuid();
    return UUID.ToString();
}

void AFatPartyPlayerState::SetPlayerID()
{
    PlayerGUID = GenerateUUID();
}

FString AFatPartyPlayerState::GetPlayerID()
{
    return PlayerGUID;
}


void AFatPartyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    DOREPLIFETIME(AFatPartyPlayerState, PlayerGUID);
}

