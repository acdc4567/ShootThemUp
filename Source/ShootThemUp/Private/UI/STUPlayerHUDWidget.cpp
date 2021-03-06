// Shoot Them Up Game.All Rights Reserved


#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const {
    
    const auto HealthComponent = GetHealthComponent() ;
    if (!HealthComponent)
        return 0.f;

    return HealthComponent->GetHealthPercent();
}


bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData &UIData) const {

    const auto WeaponComponent = GetWeaponComponent();
    if (!WeaponComponent)
        return 0;

    return WeaponComponent->GetWeaponUIData(UIData);
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData &AmmoData) const {

    const auto WeaponComponent = GetWeaponComponent();
    if (!WeaponComponent)
        return 0;

    return WeaponComponent->GetWeaponAmmoData(AmmoData);
}

bool USTUPlayerHUDWidget::GetIsPlayerAlive() const{

    const auto HealthComponent = GetHealthComponent() ;
    return HealthComponent&& !HealthComponent->IsDead();

}

bool USTUPlayerHUDWidget::GetIsPlayerSpectating() const{
    const auto Controller=GetOwningPlayer();
    return Controller&&Controller->GetStateName()==NAME_Spectating;
    
}





USTUWeaponComponent *USTUPlayerHUDWidget::GetWeaponComponent() const {
    const auto Player = GetOwningPlayerPawn();
    if (!Player)
        return nullptr;

    const auto Component = Player->GetComponentByClass(USTUWeaponComponent::StaticClass());
    const auto WeaponComponent = Cast<USTUWeaponComponent>(Component);

    return WeaponComponent;
}

USTUHealthComponent *USTUPlayerHUDWidget::GetHealthComponent() const {
    const auto Player = GetOwningPlayerPawn();
    if (!Player)
        return nullptr;

    const auto Component = Player->GetComponentByClass(USTUHealthComponent::StaticClass());
    const auto HealthComponent = Cast<USTUHealthComponent>(Component);

    return HealthComponent;
}
