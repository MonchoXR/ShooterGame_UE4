// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/RP_DoorKey.h"
#include "Components/StaticMeshComponent.h"
#include "RP_Character.h" //Si bien el padre tiene la referencua del character pero hacemos include porque no sabe que usaremos sus funciones

ARP_DoorKey::ARP_DoorKey()
{

	KeyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMeshComponenet"));
	KeyMeshComponent->SetupAttachment(RootComponent);
	KeyMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);//No queremos colisionar porque lo hace el MainCollider

	KeyTag = "keyA";
	XPValue = 40.0f;

}

void ARP_DoorKey::Pickup(ARP_Character* PickupCharacter)
{
	Super::Pickup(PickupCharacter); //Llama  y va hacer lo que hace el padre de esta funcion 




	PickupCharacter->AddKey(KeyTag);
	//PickupCharacter->GainUltimateXP(XPValue);
	Destroy();

}
