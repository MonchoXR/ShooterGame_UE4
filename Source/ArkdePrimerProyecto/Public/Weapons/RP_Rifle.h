// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/RP_Weapon.h"
#include "RP_Rifle.generated.h"


class UParticleSystem;

/**
 * 
 */
UCLASS()
class ARKDEPRIMERPROYECTO_API ARP_Rifle : public ARP_Weapon
{
	GENERATED_BODY()

public:

	ARP_Rifle();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Lincetrace|Debug")
	bool bDrawLineTrace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Lincetrace")
	float TraceLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Effects")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Effects")
	FName TraceParamName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Effects")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Effects")
	UParticleSystem* ImpacEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Effects")
	UParticleSystem* TraceEffect;
protected:

//NO es necesario el UFunction porque tiene su clase padre
virtual void StartAction() override;

virtual void StopAction() override;
	
};
