// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RP_GameMode.h"
#include "RP_Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h" //Para obtener todos los actores en una sola clase
#include "RP_SpectatingCamera.h"

void ARP_GameMode::BeginPlay()
{
	Super::BeginPlay();
	SetupSpectatingCameras();

}

void ARP_GameMode::SetupSpectatingCameras()
{

	TArray<AActor*> SpectatingCameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARP_SpectatingCamera::StaticClass(), SpectatingCameraActors);

	if (SpectatingCameraActors.Num() > 0)
	{

		for (AActor* SpectatingActor : SpectatingCameraActors)
		{
			ARP_SpectatingCamera* SpectatingCamera = Cast<ARP_SpectatingCamera>(SpectatingActor);
			if (IsValid(SpectatingCamera))
			{
				switch (SpectatingCamera->GetCameraType())
				{
				case ERP_SpectatingCameraType::CameraType_Victory:
					VictoryCamera = SpectatingCamera;
					break;

				case ERP_SpectatingCameraType::CameraType_GameOver:
					GameOverCamera = SpectatingCamera;
					break;
				default:
					break;
				}
			}

		}

	}




}

void ARP_GameMode::MoveCameraToSpectatingPoint(ARP_Character* Character, ARP_SpectatingCamera* SpectatingCamera)
{
	if (!IsValid(Character) || !IsValid(SpectatingCamera))
	{
		return;
	}
		AController* CharacterController = Character->GetController();
		if (IsValid(CharacterController))
		{
			//Vamos a castear porque los controladores pueden ser de AI o del Jugador, queremis del jugador
			APlayerController* PlayerController = Cast<APlayerController>(CharacterController);
			if (IsValid(PlayerController))
			{
				PlayerController->SetViewTargetWithBlend(SpectatingCamera, SpectatingBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);//Esta funcion mueve la camara del controlador a un punto halladamos detectado como target

			}
		}
	
}

void ARP_GameMode::Victory(ARP_Character* Character)
{
	Character->DisableInput(nullptr);

	MoveCameraToSpectatingPoint(Character, VictoryCamera);

	BP_Victory(Character);
}


void ARP_GameMode::GameOver(ARP_Character* Character)
{
	Character->GetMovementComponent()->StopMovementImmediately();
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	if (Character->HastoDestroy())
	{
		Character->DetachFromControllerPendingDestroy(); //Deshabilita el control del Personaje, no como lon inputs sino de forma separada, si el personaje muere puedes controlar el input de la camara por ejm.
		Character->SetLifeSpan(5.0f);//funcion que permite setear una duracion del objeto en la escena
	}
	else
	{
		Character->DisableInput(nullptr);
		MoveCameraToSpectatingPoint(Character, GameOverCamera);
	}
	

	BP_GameOver(Character);
}
