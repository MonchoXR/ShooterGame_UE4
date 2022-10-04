// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "RP_Character.generated.h"


class USpringArmComponent;
class UCameraComponent;
class ARP_Weapon;
class UAnimMontage;
class UAnimInstance;
class URP_HealthComponent;
class ARP_GameMode;

//ASi se crea un enum
UENUM(Blueprintable)
enum class ERP_CharacterType : uint8
{
	//CameraType_None		UMETA(DisplayName = "None"), //Siempre se debe empezar un valor por defecto, es este caso le ponemos None
	CharacterType_Player	UMETA(DisplayName = "Player"),
	CharacterType_Enemy	    UMETA(DisplayName = "Enemy")
};



UCLASS()
class ARKDEPRIMERPROYECTO_API ARP_Character : public ACharacter
{
	GENERATED_BODY()


protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	UCameraComponent* FPSCameraComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	UCameraComponent* TPSCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	UCapsuleComponent* MeleeDetectorComponent; //No necesita ForwardDeclaration ya que pior defecto viene en el character

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	URP_HealthComponent* HealthComponent;

	FTimerHandle TimerHandle_Ultimate;

	FTimerHandle TimerHandle_AutomatiShoot;

	FTimerHandle TimerHandle_BeginUltimateBehaviour;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mi Aiming") //Para que se leer en blueprint y puede ser llamada en el edtior de blueprint
	bool bUseFirsPersonView;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mi Aiming") 
	bool bIsLookInversion;

	UPROPERTY(BlueprintReadOnly, Category = "Mi Weapon")
	bool bCanUseWeapon;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mi Melee")
	bool bCanMakeCombos;

	UPROPERTY(BlueprintReadOnly, Category = "Mi Melee")
	bool bIsComboEnable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Over")
	bool bHastoDestroy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mi Ultimate")
	bool bUiltimateWithTick;

	UPROPERTY(BlueprintReadOnly, Category = "Mi Ultimate")
	bool bCanUseUltimate;

	UPROPERTY(BlueprintReadOnly, Category = "Mi Ultimate")
	bool bIsUsingUltimate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mi Melee", meta=(EditCondition = bCanMakeCombos, ClampMin = 1.0 , UIMin=1.0))
	float MaxComboMultiplier;

	UPROPERTY(BlueprintReadOnly, Category = "Mi Melee", meta = (EditCondition = bCanMakeCombos, ClampMin = 1.0, UIMin = 1.0))
	float CurrentComboMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mi Ultimate", meta = (ClampMin = 0.0, UIMin = 0.0))
	float MaxUltimateXP;

	UPROPERTY(BlueprintReadOnly, Category = "Mi Ultimate")
	float CurrentUltimateXP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mi Ultimate|Time", meta = (ClampMin = 0.0, UIMin = 0.0))
	float MaxUltimateDuration;

	UPROPERTY(BlueprintReadOnly, Category = "Mi Ultimate|Time")
	float CurrentUltimateDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mi Ultimate|Time")
	float UltimateFrequency;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mi Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
	float UltimateWalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mi Ultimate|Abilities")
	float NormalWalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mi Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
	float UltimatePlayRate;

	UPROPERTY(BlueprintReadOnly, Category = "Mi Ultimate | Abilities")
	float PlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mi Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
	float UltimateShotFrecuency;
			
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mi Melee")
	float MeleeDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Mi Melee")
	float bIsDoingMelee;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mi Aiming")
	FName FPSCameraSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mi Melee")
	FName MeleeSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mi Key")
	TArray<FName> Doorkeys;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ERP_CharacterType CharacterType;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ARP_Weapon> InitalWeaponClass; //Referencia Cuando existe en el contenedor

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	ARP_Weapon* CurrentWeapon; //Referencia Cuando ya existe en escena

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MeleeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* UltimateMontage;

	UAnimInstance* MyAnimInstance;

	ARP_GameMode* GameModeReference;

public:
	// Sets default values for this character's properties
	ARP_Character();

	virtual FVector GetPawnViewLocation() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeReferences();

	void MoveForward(float value); // como es una funcion usada en BindAxis entonces necesitamos poner paramatro de un valor

	void MoveRight(float value);

	virtual void Jump() override; //el virtual void Jump() viene de3 la clase character y se puede usar y hacer como hijo o tmb puede sobreescribir
								  //Como usamos como hijo esta funcion entonces adicionamos override
	virtual void StopJumping() override;

	void CreateInitialWeapon();

	void StartWeaponAction();

	void StopWeaponAction();


	UFUNCTION(BlueprintCallable)
	void StartMelee();


	void StopMelee();
	
	void StartUltimate();

	void StopUltimate();

	UFUNCTION()
	void MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHealthChanged(URP_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual void AddControllerPitchInput(float value) override; //podemos sobreescribir ya que esta funcion tenia el como virtual en la clase padre del character

	void AddKey(FName NewKey);

	bool TryAddHealth(float HealthToAdd);

	bool hasKey(FName KeyTag);

	void SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState);

	void SetMeleeState(bool NewState);

	UFUNCTION(BlueprintCallable)
	void SetComboEnable(bool NewState);
	
	UFUNCTION(BlueprintCallable)
	void ResetCombo(bool NewState);

	bool HastoDestroy() { return bHastoDestroy; };

	UFUNCTION(BlueprintCallable)
	void GainUltimateXP(float XPGained);

	void UpdateUltimateDuration(float Value);
	void UpdateUltimateDurationWithTimer();
	void BeginUltimateBehaviour();

	UFUNCTION(BlueprintCallable)
	ERP_CharacterType GetCharacterType() { return CharacterType; };

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GainUltimateXP(float XPGained);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StartUltimate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StopUltimate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_UpdateUltimateDuration(float Value);
};
