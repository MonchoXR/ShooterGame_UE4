// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RP_Platform.generated.h"

//class USceneComponent;
class UStaticMeshComponent;
UCLASS()
class ARKDEPRIMERPROYECTO_API ARP_Platform : public AActor
{
	GENERATED_BODY()

protected:  //Se recomienda que los componentes este en la seccion pretected


	UPROPERTY(VisibleAnywhere, Category = "My Components") //Permitira visualizar y editar las variables dentro del motor
	USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Components")
	UStaticMeshComponent* PlatformMeshComponent;

	
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Platform")
	bool bIsGoingUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Platform")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Platform")
	float MinHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Platform")
	float MaxHeight;
public:	
	// Sets default values for this actor's properties
	ARP_Platform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "My Movement")  // BlueprintCallable hace que esta funcion pueda ser llamada desde un blueprint
	void Move();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
