// Shoot Them Up Game.All Rights Reserved


#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"


void ASTURifleWeapon::StartFire() {
    
    MakeShot();
    GetWorldTimerManager().SetTimer(ShotTimerHandle,this,&ASTURifleWeapon::MakeShot,TimeBetweenShots,true);

}
void ASTURifleWeapon::StopFire() {
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);

   
}

void ASTURifleWeapon::MakeShot(){
    if(!GetWorld()||IsAmmoEmpty()) {
        StopFire();
        return;
    }

    FVector TraceStart;
    FVector TraceEnd;
    if(!GetTraceData(TraceStart,TraceEnd)) {
        StopFire();
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult,TraceStart,TraceEnd);

    if(HitResult.bBlockingHit){

        MakeDamage(HitResult);

       
    }
    else{
        
    }
    DecreaseAmmo();
    
}


bool ASTURifleWeapon::GetTraceData(FVector& TraceStart,FVector& TraceEnd) const{
    FVector ViewLocation;
    FRotator ViewRotation;
    if(!GetPlayerViewPoint(ViewLocation,ViewRotation)) return 0;


    TraceStart=ViewLocation; 
    const auto HalfRad=FMath::DegreesToRadians(BulletSpread);

    const FVector ShootDirection=FMath::VRandCone( ViewRotation.Vector(),HalfRad);                 
    TraceEnd=TraceStart+ShootDirection*TraceMaxDistance;
    return 1;
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult){
    const auto DamageActor=HitResult.GetActor();
    if(!DamageActor) return;
    DamageActor->TakeDamage(DamageAmount,FDamageEvent(),GetPlayerController(),this);;


}