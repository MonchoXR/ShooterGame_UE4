// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RP_HealthComponent.generated.h"

//Hacesmos esto para el GameOver, muere el jugador
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangeSignature, URP_HealthComponent*, HealthComponent, AActor*, DamagedActor, float, Damage, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeadSignature, AActor*, DamageCauser);

UCLASS( ClassGroup=(ROOM), meta=(BlueprintSpawnableComponent) )
class ARKDEPRIMERPROYECTO_API URP_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Debug")
	bool bDebug;

	UPROPERTY(BlueprintReadOnly, Category = "My Health Component")
	bool bIsDead;

	UPROPERTY(BlueprintReadWrite, Category = "My Health Component")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Health Component", meta=(ClampMin = 0.0, UIMin= 0.0))//meta para clampear los valores en la edicion
	float MaxHealth;

	UPROPERTY(BlueprintReadWrite, Category = "My Health Component")
	AActor* MyOwner;

public: //Publica porque sera llamado por le character para el GameOver
	UPROPERTY(BlueprintAssignable)//Con esto podemis ver los EVENTS en el editor. Este caso se motrará OnHealthChangeDelegate
	FOnHealthChangeSignature OnHealthChangeDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnDeadSignature OnDeadDelegate;

public:	
	// Sets default values for this component's properties
	URP_HealthComponent();

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return bIsDead; }; //Porque retorna el mismo valor


	bool TryAddHealth(float HealthToAdd);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION() //sus paramentros vienen del delegate OnTakeAnyDamage
	void TakingDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

};
