// Shoot Them Up Game.All Rights Reserved


#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"




// Sets default values
ASTUBasePickup::ASTUBasePickup()
{
	PrimaryActorTick.bCanEverTick = true;


	CollisionComponent=CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(50.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(CollisionComponent);


}

// Called when the game starts or when spawned
void ASTUBasePickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTUBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	check(CollisionComponent);

}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor){
	Super::NotifyActorBeginOverlap( OtherActor);
	const auto Pawn=Cast<APawn>(OtherActor);
	if(Pawn){
		if(GivePickupTo(Pawn)){
			PickupWasTaken();
		}
	}
	
	
	
}

void ASTUBasePickup::PickupWasTaken(){
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	if(GetRootComponent()){
		GetRootComponent()->SetVisibility(0,1);

	}
	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(RespawnTimerHandle,this,&ASTUBasePickup::Respawn,RespawnTime);
}

void ASTUBasePickup::Respawn(){
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	if(GetRootComponent()){
		GetRootComponent()->SetVisibility(1,1);

	}
}

bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn){
	return 0;
}