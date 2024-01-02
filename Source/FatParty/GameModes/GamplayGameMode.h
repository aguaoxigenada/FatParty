#pragma once

#include "CoreMinimal.h"
#include "FatPartyGameMode.h"
#include "GamplayGameMode.generated.h"



UCLASS()
class FATPARTY_API AGamplayGameMode : public AFatPartyGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Gameplay") // Deberia ser desde el server.?
    void SpawnPlayerPawn(AThePlayerController* NewPlayer);

	UFatPartyGameInstance* TheGameInstance;

private:
	AThePlayerController* PlayerController;
	AThePlayerController* NewPlayerController;
};
