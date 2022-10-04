// Fill out your copyright notice in the Description page of Project Settings.


#include "RP_Character.h"
#include "ArkdePrimerProyecto/ArkdePrimerProyecto.h" //Porque agragamos nuevas colisiones
#include "Camera/CameraComponent.h"
#include "Weapons/RP_Weapon.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h" //Para referencial al Mesh en el beginplay
#include "Animation/AnimInstance.h" 
#include "Animation/AnimMontage.h" 
#include "Components/CapsuleComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "Components/RP_HealthComponent.h"
#include "Core/RP_GameMode.h"
#include "GameFramework/CharacterMovementComponent.h"




// Sets default values
ARP_Character::ARP_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseFirsPersonView = true;
	FPSCameraSocketName = "SCK_Camera";
	MeleeSocketName = "SCK_Melee";
	MeleeDamage = 10.0f;
	MaxComboMultiplier = 4.0f;
	CurrentComboMultiplier = 1.0f;
	bCanUseWeapon = true;

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Mi_FPS_CameraComponent"));
	FPSCameraComponent->bUsePawnControlRotation = true; //permite que el personaje rote con e mouse
	//FPSCameraComponent->SetupAttachment(RootComponent); // Para este caso como es un character el root componenet es la capsula
	FPSCameraComponent->SetupAttachment(GetMesh(), FPSCameraSocketName); // La camara necesita ser hija del Mesh del character para usar el socket que creamos en huesos, para eso atachamos a GetMesh.

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Mi_SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Mi_TPS_CameraComponent"));
	//TPSCameraComponent->bUsePawnControlRotation = true; //permite que el personaje rote con e mouse
	TPSCameraComponent->SetupAttachment(SpringArmComponent); // La camara necesita ser hija del Mesh del character para usar el socket que creamos en huesos, para eso atachamos a GetMesh.

	MeleeDetectorComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Mi_MeleeDetectorComponent"));
	MeleeDetectorComponent->SetupAttachment(GetMesh(), MeleeSocketName);
	MeleeDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);//Primero ignaramos todos
	MeleeDetectorComponent->SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap); //luego seatamos que solo el pawn haga Overlap 
	MeleeDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthComponent = CreateDefaultSubobject<URP_HealthComponent>(TEXT("Mi_HealthComponent")); //Como es un componente abstracto no necesita atachar a un root 

	MaxUltimateXP = 100.0f;
	MaxUltimateDuration = 10.0f;
	bUiltimateWithTick = true;
	UltimateFrequency = 0.5f;

	UltimateWalkSpeed = 2000.0f;
	UltimatePlayRate = 2.0f;
	PlayRate = 1.0f;
	UltimateShotFrecuency = 0.1f;
}

FVector ARP_Character::GetPawnViewLocation() const
{
	if (IsValid(FPSCameraComponent) && bUseFirsPersonView)
	{
		return FPSCameraComponent->GetComponentLocation();
	}

	if (IsValid(TPSCameraComponent) && !bUseFirsPersonView)
	{
		return TPSCameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

// Called when the game starts or when spawned
void ARP_Character::BeginPlay()
{
	Super::BeginPlay();

	InitializeReferences();
	CreateInitialWeapon();
	MeleeDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &ARP_Character::MakeMeleeDamage);

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &ARP_Character::OnHealthChanged);

	NormalWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void ARP_Character::InitializeReferences()
{
	if (IsValid(GetMesh()))
	{
		MyAnimInstance = GetMesh()->GetAnimInstance();
	}
	
	GameModeReference = Cast<ARP_GameMode>(GetWorld()->GetAuthGameMode());//Traemos el GameMode actual que existe en la scena
}

void ARP_Character::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * value); //GetActorForwardVector solamente para Actor. AddMovementInput es una funcion propia para character

}

void ARP_Character::MoveRight(float value)
{

	AddMovementInput(GetActorRightVector() * value);
}

void ARP_Character::Jump()
{
	Super::Jump(); //Si queremos modificar o llamar estas funciones de la clase character se pone el SUPER

}

void ARP_Character::StopJumping()
{
	Super::StopJumping();
}




void ARP_Character::AddControllerPitchInput(float value)
{
	/*
	if (bIsLookInversion) {
	
		Super::AddControllerPitchInput(-value);
	
	}

	else {
		
		Super::AddControllerPitchInput(value);
	
	}

	*/

	// Lo anterio se puede simplificar de la siguiente manera

	Super::AddControllerPitchInput(bIsLookInversion ? -value : value);

	
}




// Called every frame
void ARP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bUiltimateWithTick && bIsUsingUltimate)
	{
		UpdateUltimateDuration(DeltaTime);

	}

	//UE_LOG(LogTemp, Warning, TEXT("mi booleano Meleee es %s"), bIsDoingMelee ? TEXT("true") : TEXT("false"));
	//UE_LOG(LogTemp, Warning, TEXT("mi booleano Weapon es %s"), bCanUseWeapon ? TEXT("true") : TEXT("false"));
}

// Called to bind functionality to input
void ARP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARP_Character::MoveForward); //BindAxis recibe(nomnre,objeto, funcion qye vamos a crear567(que queremos enlazar)))
	PlayerInputComponent->BindAxis("MoveRight", this, &ARP_Character::MoveRight);

	/* Comentamos porque usaremos nuestra propia direccion y sobreescribir reemplazando nuestro propio character
		
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput); //AddControllerPitchInput  funciones predeterminadas que da el pawn
	PlayerInputComponent->BindAxis("LookRight", this, &ACharacter::AddControllerYawInput);
	*/

	PlayerInputComponent->BindAxis("LookUp", this, &ARP_Character::AddControllerPitchInput);//AddControllerPitchInput  funciones predeterminadas que da el pawn pero ya sobreescribida para nuestro character
	PlayerInputComponent->BindAxis("LookRight", this, &ARP_Character::AddControllerYawInput); 

	PlayerInputComponent->BindAction("Jump",IE_Pressed ,this, &ARP_Character::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARP_Character::StopJumping);

	PlayerInputComponent->BindAction("WeaponAction", IE_Pressed, this, &ARP_Character::StartWeaponAction);
	PlayerInputComponent->BindAction("WeaponAction", IE_Released, this, &ARP_Character::StopWeaponAction);

	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &ARP_Character::StartMelee);
	PlayerInputComponent->BindAction("Melee", IE_Released, this, &ARP_Character::StopMelee);

	PlayerInputComponent->BindAction("Ultimate", IE_Pressed, this, &ARP_Character::StartUltimate);
	PlayerInputComponent->BindAction("Ultimate", IE_Released, this, &ARP_Character::StopUltimate);

}



void ARP_Character::AddKey(FName NewKey)
{
	Doorkeys.Add(NewKey);



}

bool ARP_Character::TryAddHealth(float HealthToAdd)
{
	return HealthComponent->TryAddHealth(HealthToAdd);

}

bool ARP_Character::hasKey(FName KeyTag)
{
	
	return Doorkeys.Contains(KeyTag);
}

void ARP_Character::SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState)
{
	MeleeDetectorComponent->SetCollisionEnabled(NewCollisionState);

}




void ARP_Character::CreateInitialWeapon()
{
	if (IsValid(InitalWeaponClass))
	{
		CurrentWeapon = GetWorld()->SpawnActor<ARP_Weapon>(InitalWeaponClass, GetActorLocation(), GetActorRotation());//Spawneamos el actor al mundo

		if (IsValid(CurrentWeapon))
		{
			//CurrentWeapon->SetOwner(this);//Dueno del arma es este personaje
			CurrentWeapon->SetCharacterOwner(this);//Dueno del arma es este personaje
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);//Atachamos el actor(el arma) al mesh
		}
	}
}


void ARP_Character::StartWeaponAction()
{
	if (!bCanUseWeapon)
	{
		return;

	}


	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StartAction();

		if (bIsUsingUltimate)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AutomatiShoot, CurrentWeapon, &ARP_Weapon::StartAction, UltimateShotFrecuency, true);
		}

	}
}

void ARP_Character::StopWeaponAction()
{
	if (!bCanUseWeapon)
	{
		return;

	}

	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StopAction();

		if (bIsUsingUltimate)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AutomatiShoot);
		}

	}
}

void ARP_Character::StartMelee()
{
	if (bIsDoingMelee && !bCanMakeCombos)
	{
		return;//No se va a reproducion la animacion porque no hace lo de abajo
	}

	if (bCanMakeCombos)
	{
		if (bIsDoingMelee) //necesitamos validar cuando apretamos el boton si ya puedo usar el ataque Melee para aumentar el multiplicador
		{
			if (bIsComboEnable) //Verificamos si esta disponible para aumentar el multiplicador
			{
				if (CurrentComboMultiplier < MaxComboMultiplier)
				{
					CurrentComboMultiplier++;
					SetComboEnable(false); // Para que la siguiente seccion de animacion sea encargada de volver activar el rango del combo
				}
				else 
				{
					return;

				}

			}

			else
			{
				return;//si no esta disponible no hacemos nada 
			}

		}

	}


	if (IsValid(MyAnimInstance)&& IsValid(MeleeMontage))
	{
		MyAnimInstance->Montage_Play(MeleeMontage, PlayRate);
	}

	SetMeleeState(true);

	/*UE_LOG(LogTemp, Warning, TEXT("Player starts melee action!"));*/

}

void ARP_Character::StopMelee()
{
	/*UE_LOG(LogTemp, Warning, TEXT("Player stops melee action!"));*/

}

void ARP_Character::StartUltimate()
{
	if (bCanUseUltimate && !bIsUsingUltimate) //bCanUseUltimate si la barra esta llena y no lo estamos usando
	{
		CurrentUltimateDuration = MaxUltimateDuration;

		bCanUseUltimate = false;
		

		if (IsValid(MyAnimInstance) && IsValid(UltimateMontage))
		{
			GetCharacterMovement()->MaxWalkSpeed = 0.0f;
			const float StartUltimateMontageDuration = MyAnimInstance->Montage_Play(UltimateMontage);//Montage_Play devuelve la longitud cuanto dura el montage
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_BeginUltimateBehaviour, this, &ARP_Character::BeginUltimateBehaviour, StartUltimateMontageDuration, false);
		}
		else
		{
			BeginUltimateBehaviour();
		}
	
		BP_StartUltimate();

	}

}

void ARP_Character::StopUltimate()
{
}

void ARP_Character::MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		if (OtherActor == this)
		{
			return; //Si yo soy el mismo target no hacemos nada ni daño
		}

		//En casi Si tenemos varios enemigos hacemos la siguiente evaluacion
		ARP_Character* MeleeTarget = Cast<ARP_Character>(OtherActor);
		if (IsValid(MeleeTarget))
		{
			bool bPlayerAttackingEnemy = GetCharacterType() == ERP_CharacterType::CharacterType_Player && MeleeTarget->GetCharacterType() == ERP_CharacterType::CharacterType_Enemy;
			bool bEnemyAttackingPlayer = GetCharacterType() == ERP_CharacterType::CharacterType_Enemy && MeleeTarget->GetCharacterType() == ERP_CharacterType::CharacterType_Player;
			if (bPlayerAttackingEnemy || bEnemyAttackingPlayer)
			{
				UGameplayStatics::ApplyPointDamage(OtherActor, MeleeDamage * CurrentComboMultiplier, SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
			}
		
		}
		else 
		{
			//Daño con respecto a Colision. OtherActor es quien hacemos daño
			//UGameplayStatics::ApplyPointDamage(OtherActor, MeleeDamage, SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
			UGameplayStatics::ApplyPointDamage(OtherActor, MeleeDamage * CurrentComboMultiplier, SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);

		}
		
	}

}

void ARP_Character::OnHealthChanged(URP_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (HealthComponent->IsDead() && GetCharacterType() == ERP_CharacterType::CharacterType_Player)
	{
		if (IsValid(GameModeReference))
		{
			GameModeReference->GameOver(this);

		}


	}
}

void ARP_Character::SetMeleeState(bool NewState)
{
	bIsDoingMelee = NewState;
	bCanUseWeapon = !NewState;
}

void ARP_Character::SetComboEnable(bool NewState)
{
	bIsComboEnable = NewState;

}

void ARP_Character::ResetCombo(bool NewState)
{
	SetComboEnable(false);
	CurrentComboMultiplier = 1.0f;
}

void ARP_Character::GainUltimateXP(float XPGained)
{
	if (bCanUseUltimate || bIsUsingUltimate)

	{
		return;// Si ua puedo usar el ultimate o si estoy usando ultimate entonces no hago nada con el return
		// En otras palabras, si no  puedo usar ultimate y si no lo estoy usando entonces puedo ganar experiencia
	}


	CurrentUltimateXP = FMath::Clamp(CurrentUltimateXP + XPGained, 0.0f, MaxUltimateXP);

	if (CurrentUltimateXP == MaxUltimateXP)
	{
		bCanUseUltimate = true;
	}

	BP_GainUltimateXP(XPGained);
}

void ARP_Character::UpdateUltimateDuration(float Value)
{
	CurrentUltimateDuration = FMath::Clamp(CurrentUltimateDuration - Value, 0.0f, MaxUltimateDuration);
	BP_UpdateUltimateDuration(Value);

	if (CurrentUltimateDuration == 0.0f)
	{
		bIsUsingUltimate = false;

		GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
		PlayRate = 1.0f;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AutomatiShoot);

		if (!bUiltimateWithTick)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Ultimate);

		}
		BP_StopUltimate();
	}
}

void ARP_Character::UpdateUltimateDurationWithTimer()
{
	UpdateUltimateDuration(UltimateFrequency);
}

void ARP_Character::BeginUltimateBehaviour()
{
	bIsUsingUltimate = true;
	GetCharacterMovement()->MaxWalkSpeed = UltimateWalkSpeed;
	PlayRate = UltimatePlayRate;

	if (!bUiltimateWithTick)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Ultimate, this, &ARP_Character::UpdateUltimateDurationWithTimer, UltimateFrequency, true);
	}
}

