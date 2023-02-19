#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ThePlayerController.generated.h"

UCLASS()
class FATPARTY_API AThePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AThePlayerController();
	void SetPlayerEnabledState(bool bPlayerEnabled);

	UClass* GetPlayerPawnClass();

    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

    
protected:
    /* Return The Correct Pawn Class Client-Side */
    UFUNCTION(Reliable, Client)
        void DeterminePawnClass();
    virtual void DeterminePawnClass_Implementation();

    /* Use BeginPlay to start the functionality */
    virtual void BeginPlay() override;

    /* Set Pawn Class On Server For This Controller */
    UFUNCTION(Reliable, Server, WithValidation)
        virtual void ServerSetPawn(TSubclassOf<AFatPartyCharacter> InPawnClass);
    virtual void ServerSetPawn_Implementation(TSubclassOf<AFatPartyCharacter> InPawnClass);
    virtual bool ServerSetPawn_Validate(TSubclassOf<AFatPartyCharacter> InPawnClass);

    /* Actual Pawn class we want to use */
    UPROPERTY(Replicated)
        TSubclassOf<AFatPartyCharacter> MyPawnClass;

    /* First Pawn Type To Use */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Controller")
        TSubclassOf<AFatPartyCharacter> PawnToUseA;

    /* Second Pawn Type To Use */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Controller")
        TSubclassOf<AFatPartyCharacter> PawnToUseB;

    

};
