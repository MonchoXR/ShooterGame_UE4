// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RP_BotSpawner.generated.h"

class UBillboardComponent;
class ARP_Bot;

UCLASS()
class ARKDEPRIMERPROYECTO_API ARP_BotSpawner : public AActor
{
	GENERATED_BODY()


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Components")
	UBillboardComponent* SpawnerBillboardComponent;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Spawner")
	bool bIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Spawner", meta = (UIMin = 1.0, ClampMin = 1.0))
	int MaxBotsCounter;

	UPROPERTY(BlueprintReadOnly, Category = "My Spawner")
	int CurrentBotsCounter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Spawner", meta = (UIMin = 0.1, ClampMin = 0.1))
	float TimeToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Spawner", meta = (MakeEditWidget = true))
	TArray<FVector> SpawnPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Spawner")
	TSubclassOf<ARP_Bot> BotClass;

	FTimerHandle TimerHandle_SpawnBot;

public:	
	// Sets default values for this actor's properties
	ARP_BotSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnBot();

	FVector GetSpawnPoint();

public:
	void NotifyBotDead();
};
