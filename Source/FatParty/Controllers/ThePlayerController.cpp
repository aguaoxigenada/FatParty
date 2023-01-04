#include "ThePlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"

void AThePlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
    if (bPlayerEnabled)
    {
        GetCharacter()->EnableInput(this);
    }
    else
    {
        GetCharacter()->DisableInput(this);
    }
    // bShowMouseCursor viene de la clase PlayerController, sirve para habilitar / deshabilitar el mouse.
    bShowMouseCursor = bPlayerEnabled;
}
