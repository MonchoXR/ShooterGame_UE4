// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RP_VictoryZone.generated.h"

class UBoxComponent;
class ARP_GameMode;

UCLASS()
class ARKDEPRIMERPROYECTO_API ARP_VictoryZone : public AActor
{
	GENERATED_BODY()


protected:

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "My Components")
	UBoxComponent* VictoryZoneComponent;

protected:

	ARP_GameMode* GameModeReference;

public:	
	// Sets default values for this actor's properties
	ARP_VictoryZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override; //Llamamos a este Overlap porque la colision esta en el root

};
