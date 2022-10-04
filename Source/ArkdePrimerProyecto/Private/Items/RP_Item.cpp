// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/RP_Item.h"
#include "Components/SphereComponent.h"
#include "RP_Character.h"
// Sets default values
ARP_Item::ARP_Item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MainColliderComponent"));
	MainColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);// COmo esta la colision respecto al mundo, si esta activiada, si responde a  fisica o no
	MainColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore); // Seteamos todas las respuesta e ignoramos todas
	MainColliderComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // bloqueamos world static, generalmente para objetos que tienen graves y puedan chocar al suelo
	MainColliderComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block); // bloqueamos woroldynamic
	MainColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Para que los pawn pueden entrar en el y pueda detectar
	RootComponent = MainColliderComponent;


}

// Called when the game starts or when spawned
void ARP_Item::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ARP_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ARP_Item::NotifyActorBeginOverlap(AActor* OtherActor) //NotifyActorBeginOverlap usa este overlap par actor siemre y cuando el root sea una colision
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (IsValid(OtherActor)) // Validar que el apuntador del actor no sea nulo
	{
		ARP_Character* OverlappedCharacter = Cast<ARP_Character>(OtherActor);
		//Cast<ARP_Character>(OtherActor) -> Aqui verificamos que la variable OtherActor es de tipo ARP_Character y guardamos en la varible OverlappedCharacter
		
		if (IsValid(OverlappedCharacter) && OverlappedCharacter->GetCharacterType()==ERP_CharacterType::CharacterType_Player)
		{
			Pickup(OverlappedCharacter); //Entra a esta funcion si OverlappedCharacter no es nulo.
										  //OverlappedCharacter en este casi nos da quien esta recogiendo cuando hay overlap
		}
	
	}


}

void ARP_Item::Pickup(ARP_Character* PickupCharacter)
{
	BP_Pickup(PickupCharacter);
}

