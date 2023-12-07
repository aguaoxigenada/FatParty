#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ThePlayerController.generated.h"

class UFatPartyGameInstance;
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

    UFUNCTION(NetMulticast, Reliable)
    void OpenWidgetFromServer();

	UFUNCTION(Server, Reliable)
		void SendToNextLevel();

	void SendToNextLevelClientCall();

    UFatPartyGameInstance* TheGameInstance;

    UFUNCTION(Server, Reliable)
		void ServerRespawnPlayer(AController* Controller);

    // Function to respawn a player
    void RespawnPlayer(AController* Controller);

    UFUNCTION(BlueprintImplementableEvent) 
	void PlayerRespawned(bool PlayerHasRespawned);

	/* Set Pawn Class On Server For This Controller */
    UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerSetPawn(TSubclassOf<AFatPartyCharacter> InPawnClass, bool PlayerInLobby);
    virtual void ServerSetPawn_Implementation(TSubclassOf<AFatPartyCharacter> InPawnClass, bool PlayerInLobby);
    virtual bool ServerSetPawn_Validate(TSubclassOf<AFatPartyCharacter> InPawnClass, bool PlayerInLobby);

	void StartChoosenPawns();

        /* Return The Correct Pawn Class Client-Side */
    UFUNCTION(BlueprintCallable, Client, Reliable)
        void DeterminePawnClass(bool PlayerInLobby);

    virtual void DeterminePawnClass_Implementation(bool PlayerInLobby);



protected:


    /* Use BeginPlay to start the functionality */
    virtual void BeginPlay() override;

	// The default spawn location for players?????
    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<APawn> ThePawnClass;

    /* Actual Pawn class we want to use */
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
        TSubclassOf<AFatPartyCharacter> MyPawnClass;

    /* First Pawn Type To Use */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Controller")
        TSubclassOf<AFatPartyCharacter> PawnToUseA;

    /* Second Pawn Type To Use */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Controller")
        TSubclassOf<AFatPartyCharacter> PawnToUseB;

	

private:
  
    void RestartPlayer();
    UWorld* World;
    
	FName CurrentLevelName;
};
