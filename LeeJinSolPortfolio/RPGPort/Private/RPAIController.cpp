// Fill out your copyright notice in the Description page of Project Settings.


#include "RPAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ARPAIController::HomePosKey(TEXT("HomePos"));
const FName ARPAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ARPAIController::TargetKey(TEXT("Target"));

ARPAIController::ARPAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/My/AI/BB_RPCharacter.BB_RPCharacter'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/My/AI/BT_RPCharacter.BT_RPCharacter'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ARPAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);	
}

void ARPAIController::RunAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			RPLOG(Error, TEXT("OnPossess"));
		}
	}
}

void ARPAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}