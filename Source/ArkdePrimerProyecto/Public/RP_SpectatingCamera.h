// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RP_SpectatingCamera.generated.h"

class UStaticMeshComponent;

//ASi se crea un enum
UENUM(Blueprintable)
enum class ERP_SpectatingCameraType : uint8
{
	CameraType_None		UMETA(DisplayName = "None"), //Siempre se debe empezar un valor por defecto, es este caso le ponemos None
	CameraType_Victory	UMETA(DisplayName = "Victory"),
	CameraType_GameOver UMETA(DisplayName = "GameOver")
};

UCLASS()
class ARKDEPRIMERPROYECTO_API ARP_SpectatingCamera : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentes")
	UStaticMeshComponent* SpectatingCameraComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mi Spectating Camera")
	ERP_SpectatingCameraType CameraType;


public:	
	// Sets default values for this actor's properties
	ARP_SpectatingCamera();

	ERP_SpectatingCameraType GetCameraType() { return CameraType; };
};
