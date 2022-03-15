// Shoot Them Up Game.All Rights Reserved


#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

ASTUBaseWeapon::ASTUBaseWeapon() {

    PrimaryActorTick.bCanEverTick = 0;
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}


void ASTUBaseWeapon::BeginPlay() {
    Super::BeginPlay();
}

void ASTUBaseWeapon::Fire() {
    UE_LOG(LogTemp, Warning, TEXT("FIre!!"));
}

//













//ASTUBaseWeapon
