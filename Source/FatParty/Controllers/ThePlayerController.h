#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ThePlayerController.generated.h"

class UMenuWidget;
class AFatPartyCharacter;
UCLASS()
class FATPARTY_API AThePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AThePlayerController();
	void SetPlayerEnabledState(bool bPlayerEnabled);

	UClass* GetPlayerPawnClass();

    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UUserWidget* LoadingWBP;
	TSubclassOf<class UUserWidget> LoadingWBPClass;

	UFUNCTION(Client, Reliable)
    void OpenWidget();

    UFUNCTION(Server, Reliable)
		void ServerRespawnPlayer(AController* Controller);

    // Function to respawn a player
    void RespawnPlayer(AController* Controller);

    UFUNCTION(BlueprintImplementableEvent) 
	void PlayerRespawned(bool PlayerHasRespawned);

protected:
    /* Return The Correct Pawn Class Client-Side */
    UFUNCTION(BlueprintCallable, Client, Reliable)
        void DeterminePawnClass();

    virtual void DeterminePawnClass_Implementation();

    /* Use BeginPlay to start the functionality */
    virtual void BeginPlay() override;

	// The default spawn location for players
    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<APawn> ThePawnClass;


	/* Set Pawn Class On Server For This Controller */
    UFUNCTION(Server, Reliable, WithValidation)
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
