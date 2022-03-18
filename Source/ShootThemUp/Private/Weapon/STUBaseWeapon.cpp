// Shoot Them Up Game.All Rights Reserved


#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

ASTUBaseWeapon::ASTUBaseWeapon() {

    PrimaryActorTick.bCanEverTick = 0;
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay() {
    Super::BeginPlay();
    check(WeaponMesh);
    checkf(DefaultAmmo.Bullets>0,TEXT("NoBullets"));
    checkf(DefaultAmmo.Clips>0,TEXT("NoClips"));
    CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::StartFire() {
}

void ASTUBaseWeapon::StopFire() {
}

void ASTUBaseWeapon::MakeShot() {
}

APlayerController *ASTUBaseWeapon::GetPlayerController() const {
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player)
        return nullptr;

    return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector &ViewLocation, FRotator &ViewRotation) const {
    const auto Controller = GetPlayerController();
    if (!Controller)
        return false;

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return 1;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const {
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector &TraceStart, FVector &TraceEnd) const {
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return 0;

    TraceStart = ViewLocation;

    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return 1;
}

void ASTUBaseWeapon::MakeHit(FHitResult &HitResult, const FVector &TraceStart, const FVector &TraceEnd) {

    if (!GetWorld())
        return;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ASTUBaseWeapon::DecreaseAmmo() {
    if (CurrentAmmo.Bullets == 0) {
        UE_LOG(LogTemp, Warning, TEXT("NoBullets"));
        return;
    }
    CurrentAmmo.Bullets--;
    LogAmmo();
    if (IsClipEmpty() && !IsAmmoEmpty()) {
        OnClipEmpty.Broadcast(this);
    }
}

bool ASTUBaseWeapon::IsAmmoEmpty() const {

    return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASTUBaseWeapon::IsClipEmpty() const {

    return CurrentAmmo.Bullets == 0;
}

bool ASTUBaseWeapon::CanReload() const {
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

bool ASTUBaseWeapon::IsAmmoFull() const {
    return CurrentAmmo.Clips==DefaultAmmo.Clips&&CurrentAmmo.Bullets==DefaultAmmo.Bullets;
}

bool ASTUBaseWeapon::TryToAddAmmo(int ClipsAmount)  {
    if(CurrentAmmo.Infinite||IsAmmoFull()||ClipsAmount<=0) return 0;
    if(IsAmmoEmpty()){
        CurrentAmmo.Clips=CurrentAmmo.Clips+FMath::Clamp(ClipsAmount,0,DefaultAmmo.Clips+1);
        OnClipEmpty.Broadcast(this);
    }
    else if(CurrentAmmo.Clips<DefaultAmmo.Clips){
        const auto NextClipsAmount=CurrentAmmo.Clips+ClipsAmount;
        if(DefaultAmmo.Clips-NextClipsAmount>=0){
            CurrentAmmo.Clips=NextClipsAmount;
        }
        else{
            CurrentAmmo.Clips=DefaultAmmo.Clips;
            CurrentAmmo.Bullets=DefaultAmmo.Bullets;
        }
    }
    else{
        CurrentAmmo.Bullets=DefaultAmmo.Bullets;
    }
    return 1;
}

void ASTUBaseWeapon::ChangeClip() {

    if (!CurrentAmmo.Infinite) {
        if (CurrentAmmo.Clips == 0) {
            UE_LOG(LogTemp, Warning, TEXT("NoClips"));
            return;
        }
        CurrentAmmo.Clips--;
    }
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    UE_LOG(LogTemp, Warning, TEXT("-------ChangeClip-------"));
}

void ASTUBaseWeapon::LogAmmo() {
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + "/";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(LogTemp, Warning, TEXT("%s"), *AmmoInfo);
}










//











//ASTUBaseWeapon
