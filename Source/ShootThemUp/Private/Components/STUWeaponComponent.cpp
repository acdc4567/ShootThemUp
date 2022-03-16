// Shoot Them Up Game.All Rights Reserved


#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"



USTUWeaponComponent::USTUWeaponComponent()
{
	
	PrimaryComponentTick.bCanEverTick = 0;

	
}


// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentWeaponIndex=0;
	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);

	
}

void USTUWeaponComponent::SpawnWeapons(){
	
	ACharacter* Character=Cast<ACharacter>(GetOwner());
	if(!Character||!GetWorld()) return;

	for(auto OneWeaponData:WeaponData){
		auto Weapon=GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
		if(!Weapon) continue;
		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);
		AttachWeaponToSocket(Weapon,Character->GetMesh(),WeaponArmorySocketName);

	}

	

}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon,USceneComponent* SceneComponent,const FName& SocketName) {
	if(!Weapon||!SceneComponent) return;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,false);
	Weapon->AttachToComponent(SceneComponent,AttachmentRules,SocketName);
	
}

void USTUWeaponComponent::StartFire() {
    if (!CanFire())
        return;
    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire() {
    if (!CurrentWeapon)
        return;
    CurrentWeapon->StopFire();
}

void USTUWeaponComponent::EquipWeapon(int WeaponIndex){

	if(WeaponIndex<0||WeaponIndex>=Weapons.Num()){	
		UE_LOG(LogTemp,Warning,TEXT("InvalidSlot"));
		return;


	}
	ACharacter* Character=Cast<ACharacter>(GetOwner());
	if(!Character) return;
	if(CurrentWeapon){
		CurrentWeapon->StopFire();
		AttachWeaponToSocket(CurrentWeapon,Character->GetMesh(),WeaponArmorySocketName);

	}


	CurrentWeapon=Weapons[WeaponIndex];
	
	const auto CurrentWeaponData=WeaponData.FindByPredicate([&](const FWeaponData& Data){return Data.WeaponClass==CurrentWeapon->GetClass();});
	CurrentReloadAnimMontage=CurrentWeaponData?CurrentWeaponData->ReloadAnimMontage:nullptr;

	AttachWeaponToSocket(CurrentWeapon,Character->GetMesh(),WeaponEquipSocketName);
	bEquipAnimInProgress=1;
	PlayAnimMontage(EquipAnimMontage);
}


void USTUWeaponComponent::NextWeapon() {
	if(!CanEquip() ) return;
	CurrentWeaponIndex=(CurrentWeaponIndex+1)%Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
	
	

}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason){
	CurrentWeapon=nullptr;
	for(auto Weapon:Weapons){
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();

	}
	Weapons.Empty();

	Super::EndPlay(EndPlayReason);
}


void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation){

	ACharacter* Character=Cast<ACharacter>(GetOwner());
	if(!Character) return;
	Character->PlayAnimMontage(Animation);




}

void USTUWeaponComponent::InitAnimations(){
	

	auto EquipFinishedNotify=FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
	if(EquipFinishedNotify){
		EquipFinishedNotify->OnNotified.AddUObject(this,&USTUWeaponComponent::OnEquipFinished);
		
	}
	for(auto OneWeaponData:WeaponData){
        auto ReloadFinishedNotify = FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (!ReloadFinishedNotify) continue;
		ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
        
    }

}
void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent){
	ACharacter* Character=Cast<ACharacter>(GetOwner());
	if((!Character||(MeshComponent!=Character->GetMesh()))) return;
	
	bEquipAnimInProgress=0;
	
	
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent){
	ACharacter* Character=Cast<ACharacter>(GetOwner());
	if((!Character||(MeshComponent!=Character->GetMesh()))) return;
	
	bReloadAnimInProgress=0;
	
	
}

bool USTUWeaponComponent::CanFire() const {

    return CurrentWeapon && !bEquipAnimInProgress && !bReloadAnimInProgress;
}
bool USTUWeaponComponent::CanEquip() const {

    return !bEquipAnimInProgress && !bReloadAnimInProgress;
}
bool USTUWeaponComponent::CanReload() const {

    return CurrentWeapon && !bEquipAnimInProgress && !bReloadAnimInProgress;
}

void USTUWeaponComponent::Reload(){
	if(!CanReload()) return;
	bReloadAnimInProgress=1;
	PlayAnimMontage(CurrentReloadAnimMontage)	;



}









//













//USTUWeaponComponent
