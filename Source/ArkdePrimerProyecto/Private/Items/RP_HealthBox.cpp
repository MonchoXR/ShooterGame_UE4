// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/RP_HealthBox.h"
#include "Components/StaticMeshComponent.h"
#include "RP_Character.h"

ARP_HealthBox::ARP_HealthBox()
{

	HealthBoxMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealtBoxComponent"));
	HealthBoxMeshComponent->SetupAttachment(RootComponent);
	HealthBoxMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthValue = 15.0f;
}

void ARP_HealthBox::Pickup(ARP_Character* PickupCharacter)
{
	Super::Pickup(PickupCharacter);

	bool bSuccessfullHealth =  PickupCharacter->TryAddHealth(HealthValue);

	if (bSuccessfullHealth)
	{
		Destroy();
	}

	

}
