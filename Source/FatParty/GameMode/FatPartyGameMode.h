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

    // Array of spawn points for players
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    TArray<APlayerStart*> PlayerStartPoints;

	void PopulatePlayerStartArray();

public:

	UFUNCTION(BlueprintCallable, Category = "Asset Management")
		void GetAssetsInLevel(ULevel* TargetLevel, TArray<UObject*>& OutAssets);
		
	//void GetAssetsInLevel(FName LevelName, FString PackagePathString, TArray<UObject*>& OutAssets);
		
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
