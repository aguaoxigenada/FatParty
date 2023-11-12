#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "FatPartyGameMode.generated.h"

class UFatPartyGameInstance;
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

    // Array of spawn points for players
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    TArray<APlayerStart*> PlayerStartPoints;

	void PopulatePlayerStartArray();

public:

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void SendToNextLevel();

	UFUNCTION(Server, Reliable)
		void LoadWBPLoadingOnClient();

	void ActorDied(AActor* DeadActor);

	//UFUNCTION(Client, Reliable)
	void OpenClientWidget();

	class ABaseCharacter* BaseCharacter;

	AFatPartyGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* SpawnPoint = nullptr;

	UFatPartyGameInstance* GameInstance;

private:
	AFatPartyCharacter* FatPartyCharacter;
	AThePlayerController* PlayerController;

	float StartDelay = 3.f;

	void HandleGameStart();	

	int32 TargetTowers = 0;
	int32 GetTargetTowerCount();

	FTimerHandle TimerHandle;
	float TimeToStart = 10;

};
