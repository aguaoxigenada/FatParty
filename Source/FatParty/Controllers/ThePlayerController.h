#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ThePlayerController.generated.h"

UCLASS()
class FATPARTY_API AThePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	void SetPlayerEnabledState(bool bPlayerEnabled);
};
