// Shoot Them Up Game.All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged,float);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTUHealthComponent();

	float GetHealth() const {return Health;}

	UFUNCTION(BlueprintCallable,Category="Health")
	bool IsDead() const {return FMath::IsNearlyZero(Health);}

	UFUNCTION(BlueprintCallable,Category="Health")
	float GetHealthPercent() const; 


	FOnDeath OnDeath;

	FOnHealthChanged OnHealthChanged;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Health",meta=(ClampMin="0.0",ClampMax="1000.0"))
	float MaxHealth=100.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Heal")
	bool AutoHeal=1;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Heal",meta=(EditCondition="AutoHeal"))
	float HealUpdateTime=1.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Heal",meta=(EditCondition="AutoHeal"))
	float HealDelay=3.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Heal",meta=(EditCondition="AutoHeal"))
	float HealModifier=5.f;


	

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	


public:	
	

private:

	float Health=0.f;

	FTimerHandle HealTimerHandle;
	void HealUpdate();
	void SetHealth(float NewHealth);


		
};
