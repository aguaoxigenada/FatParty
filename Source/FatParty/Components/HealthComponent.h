#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UFatPartyGameInstance;
class UHudWidget;
class AFatPartyGameMode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnObjectDamaged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FATPARTY_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void AddHealth(float HealthToAdd);

	UFUNCTION(BlueprintCallable)
		float GetHealth();

	UPROPERTY()
		FOnHealthChange OnHealthChange;

	UPROPERTY()
		FOnObjectDamaged OnObjectDamaged;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
		float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere)
		float Health = 0.f;

	UPROPERTY(EditAnywhere)
		float Delay = 2.f;

	UFUNCTION(NetMulticast, Reliable)
		void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser);

	UFUNCTION(Client, Reliable)
		void OpenInGameMenu();

	UHudWidget* HudWidget;
	UFatPartyGameInstance* GameInstance;
	AFatPartyGameMode* FatPartyGameMode;
	

	bool bPlayerAlive = true;
			
};
