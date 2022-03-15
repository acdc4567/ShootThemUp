// Shoot Them Up Game.All Rights Reserved


#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/STUWeaponComponent.h"
#include "GameFramework/Controller.h"



DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog,All,All);


// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit):
Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent=CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation=1;
	SpringArmComponent->SocketOffset=FVector(0.f,100.f,80.f);


	CameraComponent=CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent=CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");
	

	HealthTextComponent=CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOwnerNoSee(1);

	WeaponComponent=CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");

}


void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(HealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement());

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this,&ASTUBaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this,&ASTUBaseCharacter::OnHealthChanged);
	LandedDelegate.AddDynamic(this,&ASTUBaseCharacter::OnGroundLanded);


}


void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

	
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	check(WeaponComponent);
	
	PlayerInputComponent->BindAxis("MoveForward",this,&ASTUBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ASTUBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn",this,&ASTUBaseCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp",this,&ASTUBaseCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ASTUBaseCharacter::Jump);
	PlayerInputComponent->BindAction("Run",IE_Pressed,this,&ASTUBaseCharacter::StartRunning);
	PlayerInputComponent->BindAction("Run",IE_Released,this,&ASTUBaseCharacter::StopRunning);
	PlayerInputComponent->BindAction("Fire",IE_Pressed,WeaponComponent,&USTUWeaponComponent::Fire);
	

}

void ASTUBaseCharacter::MoveForward(float Amount) {
	
	bIsMovingForward=Amount>0.f;
	if(Amount==0.f) return;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUBaseCharacter::MoveRight(float Amount) {
	if(Amount==0.f) return;
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::StartRunning(){
	
	bWantsToRun=1;

}
void ASTUBaseCharacter::StopRunning(){
	bWantsToRun=0;

}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit ){
	const auto FallZVelocity=-GetCharacterMovement()->Velocity.Z;
	if (FallZVelocity<LandedDamageVelocity.X) return;
	const auto FinalDamage=FMath::GetMappedRangeValueClamped(LandedDamageVelocity,LandedDamage,FallZVelocity);
	TakeDamage(FinalDamage,FDamageEvent{},nullptr,nullptr);

}

void ASTUBaseCharacter::OnDeath(){
	UE_LOG(LogTemp,Warning,TEXT("OPlayer %s IsDead"),*GetName());
	PlayAnimMontage(DeathAnimMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.f);
	if(Controller){
		Controller->ChangeState(NAME_Spectating);

	}

}

void ASTUBaseCharacter::OnHealthChanged(float Health){
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"),Health)));

}

bool ASTUBaseCharacter::IsRunning() const{
	return bWantsToRun&&bIsMovingForward&&!GetVelocity().IsZero();

}



float ASTUBaseCharacter::GetMovementDirection() const{
	if(GetVelocity().IsZero()) return 0.f;
	const auto VelocityNormal=GetVelocity().GetSafeNormal();
	const auto AngleBetween=FMath::Acos(FVector::DotProduct(GetActorForwardVector(),VelocityNormal));
	const auto CrossProduct=FVector::CrossProduct(GetActorForwardVector(),VelocityNormal);
	const auto Degrees=FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero()?Degrees:Degrees* FMath::Sign(CrossProduct.Z);


}





//


















//ASTUBaseCharacter
