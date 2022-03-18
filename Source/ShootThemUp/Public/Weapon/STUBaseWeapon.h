// Shoot Them Up Game.All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature,ASTUBaseWeapon*);


class USkeletalMeshComponent;


USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_USTRUCT_BODY()

	

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Weapon")
	UTexture2D* MainIcon;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Weapon")
	UTexture2D* CrossHairIcon;

	
};


USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Weapon")
	int Bullets;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Weapon")
	int Clips;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Weapon")
	bool Infinite;
};




UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASTUBaseWeapon();
	virtual void StartFire();
	virtual void StopFire();

	FOnClipEmptySignature OnClipEmpty;

	void ChangeClip();
	bool CanReload() const;

	FWeaponUIData GetUIData() const {return UIData;}
	FAmmoData GetAmmoData() const {return CurrentAmmo;}

	bool TryToAddAmmo(int ClipsAmount);


protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	FName MuzzleSocketName="MuzzleFlashSocket";

	float TraceMaxDistance=50000.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DamageAmount=10.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Weapon")
	FAmmoData DefaultAmmo{15,10,0};

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="UI")
	FWeaponUIData UIData;

	virtual void MakeShot();

	APlayerController* GetPlayerController() const;
	bool GetPlayerViewPoint(FVector& ViewLocation,FRotator& ViewRotation) const;
	FVector GetMuzzleWorldLocation() const;
	virtual bool GetTraceData(FVector& TraceStart,FVector& TraceEnd) const;
	void MakeHit(FHitResult& HitResult,const FVector& TraceStart,const FVector& TraceEnd);
	
	void DecreaseAmmo();
	bool IsAmmoEmpty() const;
	bool IsClipEmpty() const;
	bool IsAmmoFull() const;
	
	
	void LogAmmo();

private:	
	FAmmoData CurrentAmmo;




	

	
};
