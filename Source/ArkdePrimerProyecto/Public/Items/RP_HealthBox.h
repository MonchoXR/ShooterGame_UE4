// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/RP_Item.h"
#include "RP_HealthBox.generated.h"

/**
 * 
 */
UCLASS()
class ARKDEPRIMERPROYECTO_API ARP_HealthBox : public ARP_Item
{
	GENERATED_BODY()
	

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentes")
		UStaticMeshComponent* HealthBoxMeshComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mi Health Box")
		float HealthValue;


public:
	// Sets default values for this actor's properties
	ARP_HealthBox();

public:
	UFUNCTION(BlueprintCallable, Category = "Mi Getter")
	float GetHealthValue() const { return HealthValue; }; 
protected:

	virtual void Pickup(ARP_Character* PickupCharacter) override;
};
