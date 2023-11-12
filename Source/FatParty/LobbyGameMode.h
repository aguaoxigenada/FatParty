#pragma once

#include "CoreMinimal.h"
#include "GameMode/FatPartyGameMode.h"
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
	void StartPlayerGame();

	UFatPartyGameInstance* GameInstance;

private:
	uint32 PlayersLoggedIn;
	FTimerHandle TimerHandle;
	float TimeToStart = 10;
	
	AThePlayerController* Controller;
};
