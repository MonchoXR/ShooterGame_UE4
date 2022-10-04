// Fill out your copyright notice in the Description page of Project Settings.


#include "RP_Door.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "RP_Character.h"

// Sets default values
ARP_Door::ARP_Door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrameComponent->SetupAttachment(CustomRootComponent);

	DoorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	DoorComponent->SetupAttachment(CustomRootComponent);

	KeyZoneColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("KeyZoneCollider"));
	KeyZoneColliderComponent->SetupAttachment(CustomRootComponent);
	KeyZoneColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	//Habilitamos Collision														
	KeyZoneColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);//Ignoramos Canales
	KeyZoneColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);//Habilitamos el Pawn y sea Overlaps

	OpenAngle = -90.0f;
	DoorTag = "KeyA";
}

// Called when the game starts or when spawned
void ARP_Door::BeginPlay()
{
	Super::BeginPlay();

	//Queremos este mismo actor y la funcion.Le estamos diciendo al colisionador que si alguien overlapee con alguien llama a la funcion CheckKeyFromPlayer 
	//Ademas siempre va en el begin
	KeyZoneColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &ARP_Door::CheckKeyFromPlayer);
}

void ARP_Door::CheckKeyFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsOpen)
	{
		return; //Si esta abiera la puerta retornar y ya no hace lo de abajo
	}

	if (IsValid(OtherActor)) // Validar que el apuntador del actor no sea nulo
	{
		ARP_Character* OverlappedCharacter = Cast<ARP_Character>(OtherActor);
		//Cast<ARP_Character>(OtherActor) -> Aqui verificamos que la variable OtherActor es de tipo ARP_Character y guardamos en la varible OverlappedCharacter
		if (IsValid(OverlappedCharacter) && OverlappedCharacter->GetCharacterType() == ERP_CharacterType::CharacterType_Player)
		{
			
			if (OverlappedCharacter->hasKey(DoorTag))
			{
				OpenDoor();

			}


		}

	}



}

// Called every frame
void ARP_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARP_Door::OpenDoor()
{
	//FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);
	//DoorComponent->SetRelativeRotation(NewRotation);
	bIsOpen = true;
	BP_OpenDoor();
	
}

