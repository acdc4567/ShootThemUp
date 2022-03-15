// Shoot Them Up Game.All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"


class ASTUBaseWeapon;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUWeaponComponent();

	void Fire();
protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,Category="Movement")
	TSubclassOf<ASTUBaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly,Category="Movement")
	FName WeaponAttachPointName="WeaponSocket";
private:	

	UPROPERTY()
	ASTUBaseWeapon* CurrentWeapon;

	void SpawnWeapon();

	


		
};
