// Fill out your copyright notice in the Description page of Project Settings.

#include "ThePlayerController.h"
#include "GameFramework/Pawn.h"

void AThePlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
    // Habilita o deshabilita los controles.

    if (bPlayerEnabled)
    {

        GetPawn()->EnableInput(this);
    }
    else
    {
        GetPawn()->DisableInput(this);
    }
    // bShowMouseCursor viene de la clase PlayerController, sirve para habilitar / deshabilitar el mouse.
    bShowMouseCursor = bPlayerEnabled;
}
