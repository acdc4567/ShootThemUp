// Shoot Them Up Game.All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "STUWeaponComponent.generated.h"





USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Weapon")
	TSubclassOf<ASTUBaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Weapon")
	UAnimMontage* ReloadAnimMontage;
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUWeaponComponent();

	void StartFire();
	void StopFire();
	void NextWeapon();
	void Reload();
	bool GetWeaponUIData(FWeaponUIData& UIData) const;
	bool GetWeaponAmmoData(FAmmoData& AmmoData) const;

	bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType,int ClipsAmount);


protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,Category="Movement")
	TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly,Category="Movement")
	FName WeaponEquipSocketName="WeaponSocket";

	UPROPERTY(EditDefaultsOnly,Category="Movement")
	FName WeaponArmorySocketName="ArmorySocket";

	UPROPERTY(EditDefaultsOnly,Category="Animation")
	UAnimMontage* EquipAnimMontage;

	void AttachWeaponToSocket(ASTUBaseWeapon* Weapon,USceneComponent* SceneComponent,const FName& SocketName);
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:	

	UPROPERTY()
	ASTUBaseWeapon* CurrentWeapon;

	UPROPERTY()
	TArray<ASTUBaseWeapon*> Weapons;

	UPROPERTY()
	UAnimMontage* CurrentReloadAnimMontage=nullptr;


	int CurrentWeaponIndex=0;

	bool bEquipAnimInProgress=0;
	bool bReloadAnimInProgress=0;

	void SpawnWeapons();

	void EquipWeapon(int WeaponIndex);

	void PlayAnimMontage(UAnimMontage* Animation);
	void InitAnimations();
	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
	void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

	bool CanFire() const;
	bool CanEquip() const;
	bool CanReload() const;

	void OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon);
	void ChangeClip();

	template<typename T>
	T* FindNotifyByClass(UAnimSequenceBase* Animation){
        if (!Animation)
            return nullptr;
        const auto NotifyEvents = Animation->Notifies;
        for (auto NotifyEvent : NotifyEvents) {
            auto AnimNotify = Cast<T>(NotifyEvent.Notify);
            if (AnimNotify) {
                return AnimNotify;
            }
        }
		return nullptr;
    }
};
