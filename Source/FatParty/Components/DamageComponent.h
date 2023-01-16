#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FATPARTY_API UDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDamageComponent();
	AActor* Parent = nullptr;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);
	
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
		float Damage = 50.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<class UCameraShakeBase>HitCameraShakeClass;

};
