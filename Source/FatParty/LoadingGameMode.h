#pragma once

#include "CoreMinimal.h"
#include "GameMode/FatPartyGameMode.h"
#include "LoadingGameMode.generated.h"


UCLASS()
class FATPARTY_API ALoadingGameMode : public AFatPartyGameMode
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
		void TravelToLevel();
};
