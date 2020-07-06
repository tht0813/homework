// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "RPAIController.h"
#include "RPCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto RPCharacter = Cast<ARPCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == RPCharacter) return EBTNodeResult::Failed;

	auto Target = Cast<ARPCharacter>(OwnerComp.GetBlackboardComponent()
		->GetValueAsObject(ARPAIController::TargetKey));
	if (nullptr == Target) return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - RPCharacter->GetActorLocation();
	LookVector.Z = 0.0f;

	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	RPCharacter->SetActorRotation(FMath::RInterpTo(
		RPCharacter->GetActorRotation(),
		TargetRot,
		GetWorld()->GetDeltaSeconds(),
		2.0f
	));

	return EBTNodeResult::Succeeded;
}

