#pragma once

#include "CoreMinimal.h"
#include "FatPartyGameMode.h"
#include "LobbyGameMode.generated.h"

class UFatPartyGameInstance;
class UMenuWidget;
UCLASS()
class FATPARTY_API ALobbyGameMode : public AFatPartyGameMode
{
	GENERATED_BODY()

public:

	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(AController* Exiting) override;

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void StartPlayerGame();

	UFatPartyGameInstance* GameInstance;

	UFUNCTION(BlueprintCallable, Category = "Lobby")
    void HandlePlayerTransitionToGame();

	APlayerController* PlayerController;


private:
	uint32 PlayersLoggedIn;
	FTimerHandle TimerHandle;
	float TimeToStart = 10;
	
	AThePlayerController* Controller;
};
