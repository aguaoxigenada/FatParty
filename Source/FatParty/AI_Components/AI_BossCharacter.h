#pragma once

#include "CoreMinimal.h"
#include "AI_CharacterBase.h"
#include "AI_BossCharacter.generated.h"

class UHealthComponent;

UCLASS()
class FATPARTY_API AAI_BossCharacter : public AAI_CharacterBase
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	int BossStateValue = 0;

	UHealthComponent* GetHealthComponent();

private:
	UHealthComponent* HealthCompt = nullptr;

	

};
