// Shoot Them Up Game.All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;


UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUBasePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere,Category="Pickup")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Pickup")
	float RespawnTime=5.f;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual bool GivePickupTo(APawn* PlayerPawn) ;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:

	void PickupWasTaken();
	void Respawn();
};
