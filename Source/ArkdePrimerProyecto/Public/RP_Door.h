// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RP_Door.generated.h"

//ponemos class y la clase que estamos usando
class USceneComponent;
class UStaticMeshComponent;  
class UBoxComponent;
UCLASS()
class ARKDEPRIMERPROYECTO_API ARP_Door : public AActor
{
	GENERATED_BODY()
protected:

	//Para todo tipo de componente se empieza la letra U
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentes") //Permitira visualizar y editar las variables dentro del motor
	USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentes")
	UStaticMeshComponent* DoorFrameComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentess")
	UStaticMeshComponent* DoorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentess")
	UBoxComponent* KeyZoneColliderComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Door") //Permita editar en el blueprint y en la scena principal o en lo niveles
	float OpenAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Door")
	bool bIsOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Door")
	FName DoorTag;


public:	
	// Sets default values for this actor's properties
	ARP_Door();






protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//creamos una funcion que va bindear a un delegate
	UFUNCTION()
	void CheckKeyFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "My Door")
	void BP_OpenDoor();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OpenDoor();


};
