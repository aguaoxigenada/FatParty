#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UHudWidget;
class AFatPartyGameMode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDamaged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FATPARTY_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		float GetHealth();

	UPROPERTY()
		FOnPlayerDamaged OnPlayerDamaged;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
		float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere)
		float Health = 0.f;
	
	UFUNCTION()
		void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser);

	UHudWidget* HudWidget;

	AFatPartyGameMode* FatPartyGameMode;
			
};
