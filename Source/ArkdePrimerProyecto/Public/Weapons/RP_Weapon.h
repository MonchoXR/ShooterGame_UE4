// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RP_Weapon.generated.h"

class UDamageType;
class ACharacter;

UCLASS()
class ARKDEPRIMERPROYECTO_API ARP_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARP_Weapon();


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	ACharacter* CurrentOwnerCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//Ojo: aqui no es necesario hacer override en el hijo porque se usara en blueprint
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon") 
	void BP_StartAction();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon")
	void BP_StopAction();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable) 
		virtual void StartAction() ; //Ponermos virtual para que estas funciones puedan los hijos suscribir o reemplazar

	UFUNCTION(BlueprintCallable)
		virtual void StopAction() ;

	UFUNCTION(BlueprintCallable)
	void SetCharacterOwner(ACharacter* NewOwner);

};
