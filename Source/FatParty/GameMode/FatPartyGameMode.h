#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "FatPartyGameMode.generated.h"

class UMenuWidget;
class AFatPartyCharacter;
class AThePlayerController;

UCLASS()
class FATPARTY_API AFatPartyGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	UFUNCTION(BlueprintImplementableEvent)  // No es necesario crearla en cpp cuando es un BlueprintImplementableEvent
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent) 
	void GameOver(bool bWonGame);

	TSubclassOf<class UUserWidget> LoadingClass;
	TSubclassOf<class UUserWidget> NetworkErrorClass;

	UMenuWidget* NetworkError;
	UUserWidget* LoadingWidget;


	//


	

    // Array of spawn points for players
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    TArray<APlayerStart*> PlayerStartPoints;
	//So, I was in the middle of typing the question and the duck answered me:
	//I am trying to put an instance of something in the level into the blueprint which is not yet instanced!


    

	void PopulatePlayerStartArray();

public:

	//UFUNCTION(Server, Reliable)
	//void ServerRespawnPlayer(AController* Controller);

	// Function to respawn a player
   // void RespawnPlayer(AController* Controller);

	// Function to choose a spawn point for a player
    virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void ActorDied(AActor* DeadActor);

	class ABaseCharacter* BaseCharacter;

	AFatPartyGameMode();


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* SpawnPoint = nullptr;

private:
	AFatPartyCharacter* FatPartyCharacter;
	AThePlayerController* PlayerController;

	float StartDelay = 3.f;

	void HandleGameStart();	

	int32 TargetTowers = 0;
	int32 GetTargetTowerCount();
};
