// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Controller/RP_AIController.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Enemy/RP_Enemy.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "AIModule/Classes/Perception/AIPerceptionComponent.h"

ARP_AIController::ARP_AIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	LooPathParametername = "bLoopPath";
	DirectionIndexParameterName = "DirectionIndex";
	WaitingTimeParameterName = "WaitingOnPathPoint";
	CanSeePlayerParameterName = "bCanSeePlayer";
	InvestigationParameterName = "bIsInvestigating";
	TargetLocationParameterName = "TargetLocation";
}
void ARP_AIController::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(EnemyBehaviortree))
	{
		RunBehaviorTree(EnemyBehaviortree);
	}

	MyEnemy = Cast<ARP_Enemy>(K2_GetPawn()); //Queremos el getcontroller pero en unreal de su codigo AIController lo llama comoK2_GetPawn
	if (IsValid(MyEnemy))
	{
		MyBlackBoard = UAIBlueprintHelperLibrary::GetBlackboard(this); //Para obtener el blackboard
		if (IsValid(MyBlackBoard))
		{
			MyBlackBoard->SetValueAsBool(LooPathParametername, MyEnemy->GetLoopPath());
			MyBlackBoard->SetValueAsInt(DirectionIndexParameterName, MyEnemy->GetDirectionIndex());
			MyBlackBoard->SetValueAsFloat(WaitingTimeParameterName, MyEnemy->GetWaitingTime());
		}
	}


	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ARP_AIController::UpdateSenses);
}

void ARP_AIController::UpdateSenses(const TArray<AActor*>& UpdateActors)
{
	if (!IsValid(Blackboard))
	{
		return;
	}

	for (AActor* Actor : UpdateActors)
	{
		FActorPerceptionBlueprintInfo PerceptionInfo;
		AIPerceptionComponent->GetActorsPerception(Actor, PerceptionInfo);

		ARP_Character* SensedCharacter = Cast<ARP_Character>(Actor);
		if (IsValid(SensedCharacter) && SensedCharacter->GetCharacterType() == ERP_CharacterType::CharacterType_Player)
		{
			for (int i = 0; i < PerceptionInfo.LastSensedStimuli.Num(); i++)
			{
				switch (i)
				{
				case 0:
					MyBlackBoard->SetValueAsBool(CanSeePlayerParameterName, PerceptionInfo.LastSensedStimuli[i].WasSuccessfullySensed());

					break;
				case 1:
					MyBlackBoard->SetValueAsBool(InvestigationParameterName, bReceivingDamage);
					if (bReceivingDamage)
					{
						MyBlackBoard->SetValueAsVector(TargetLocationParameterName, PerceptionInfo.LastSensedStimuli[i].StimulusLocation);
					}

					break;
				default:
					break;
				}
			}

		}

	}
}
