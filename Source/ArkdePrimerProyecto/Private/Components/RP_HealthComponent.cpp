// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RP_HealthComponent.h"

// Sets default values for this component's properties
URP_HealthComponent::URP_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false; //le ponemos false para que no utilice el tick

	MaxHealth = 100.0f;
}


// Called when the game starts
void URP_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Health = MaxHealth; // Cuando empice a correr la salud inicial sera igual salud Maxima

	MyOwner = GetOwner();
	if (IsValid(MyOwner))
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &URP_HealthComponent::TakingDamage); // el TakingDamage debe tener los mismos parametros del OnTakeAnyDamage por ser delegates

	}


}

void URP_HealthComponent::TakingDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
	{
		return; //Retorna y no hace lo de abajo

	}

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);// (Health - Damage) es el resultado que vamos a recibir el impacto

	if (Health == 0.0f)
	{
		bIsDead = true;
		OnDeadDelegate.Broadcast(DamageCauser);

	}

	//Para llamar al delegate se usa Broadcast, y dentro de ella viene todos los parametros de la firma
	OnHealthChangeDelegate.Broadcast(this, DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);

	if (bDebug)
	{
		//UE_LOG(LogTemp, Log, TEXT("My Healt is: %f"), Health);
		UE_LOG(LogTemp, Log, TEXT("My Healt is: %s"), *FString::SanitizeFloat(Health));
	}


}

bool URP_HealthComponent::TryAddHealth(float HealthToAdd)
{

	if (bIsDead)
	{
		return false;
	}

	if (Health == MaxHealth)
	{
		return false;
	}

	Health = FMath::Clamp(Health + HealthToAdd, 0.0f, MaxHealth);

	if (bDebug)
	{
		//UE_LOG(LogTemp, Log, TEXT("My Healt is: %f"), Health);
		UE_LOG(LogTemp, Log, TEXT("My Healt is: %s"), *FString::SanitizeFloat(Health));
	}

	return true;
}



