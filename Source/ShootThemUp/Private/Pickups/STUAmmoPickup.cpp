// Shoot Them Up Game.All Rights Reserved


#include "Pickups/STUAmmoPickup.h"
#include "Weapon/STUBaseWeapon.h"
#include "Components/STUWeaponComponent.h"
#include "Components/STUHealthComponent.h"

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn) {
    
    //Super::GivePickupTo(PlayerPawn);
    const auto Component = PlayerPawn->GetComponentByClass(USTUHealthComponent::StaticClass());
    const auto HealthComponent = Cast<USTUHealthComponent>(Component);
    if(!HealthComponent||HealthComponent->IsDead()) return 0;

    const auto Componentx1 = PlayerPawn->GetComponentByClass(USTUWeaponComponent::StaticClass());
    const auto WeaponComponent = Cast<USTUWeaponComponent>(Componentx1);
    if(!WeaponComponent) return 0;
    return WeaponComponent->TryToAddAmmo(WeaponType,ClipsAmount);
}