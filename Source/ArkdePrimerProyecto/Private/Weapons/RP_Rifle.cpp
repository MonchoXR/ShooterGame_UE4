// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/RP_Rifle.h"
#include "ArkdePrimerProyecto/ArkdePrimerProyecto.h"
#include "DrawDebugHelpers.h" //Para dibujar las lineas de trace
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"  //Llamos aqui por el traceEffect
#include "Components/SkeletalMeshComponent.h"

ARP_Rifle::ARP_Rifle() {

	TraceLength = 10000.0f;
	MuzzleSocketName = "SCK_Muzzle";
}


void ARP_Rifle::StartAction()

{
	Super::StartAction();


	AActor* CurrentOwner = GetOwner();

	if (IsValid(CurrentOwner)) 
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		FVector ShotDirection = EyeRotation.Vector();

		//CurrentOwnerCharacter->GetActorEyesViewPoint(EyeLocation, EyeRotation);?
		CurrentOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);//GetActorEyesViewPoint permite regresaron al punto de vista de locacion y rotacion del actor que estamos manipulando
		FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * TraceLength);
		
		FCollisionQueryParams QueryParams; //Para anadir actores como parametros para que se ignoren
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(CurrentOwner);
		QueryParams.bTraceComplex = true; //Por defecto es falso, nos da una informacion adicional de la colision

		FVector TraceEndPoint = TraceEnd;

		FHitResult HitResult;
		bool bhit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams);

		if (bhit)
		{
			//Hace daño
			AActor* HitActor = HitResult.GetActor(); //Obtenemos el actor que impactó
			if (IsValid(HitActor))
			{
				UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, HitResult, CurrentOwner->GetInstigatorController(), this, DamageType);
					
			}

			TraceEndPoint = HitResult.ImpactPoint;

			if (IsValid(ImpacEffect))
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpacEffect, TraceEndPoint, HitResult.ImpactNormal.Rotation());
			}

		

		}

		if (bDrawLineTrace)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0.0f, 1.0f);

		}

		if (IsValid(MuzzleEffect))//MuzzleEffect es el ParticleSystem
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, CurrentOwnerCharacter->GetMesh(), MuzzleSocketName);
		}

		if (IsValid(TraceEffect))
		{
			USkeletalMeshComponent* CharacterMeshComponent = CurrentOwnerCharacter->GetMesh();

			if (IsValid(CharacterMeshComponent))
			{
				FVector MuzzleSocketLocation = CharacterMeshComponent->GetSocketLocation(MuzzleSocketName);
				UParticleSystemComponent* TracerComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect, MuzzleSocketLocation);
				
				if (IsValid(TracerComponent))
				{
					TracerComponent->SetVectorParameter(TraceParamName, TraceEndPoint);

				}
			}
		}

	}

	//UE_LOG(LogTemp, Log, TEXT("Player starts Action !"));
}

void ARP_Rifle::StopAction()
{
	Super::StopAction();
	//UE_LOG(LogTemp, Log, TEXT("Player stop Action !"));
}


