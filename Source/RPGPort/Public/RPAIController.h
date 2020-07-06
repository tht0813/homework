// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGPort.h"
#include "AIController.h"
#include "RPAIController.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORT_API ARPAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ARPAIController();
	virtual void OnPossess(APawn* InPawn) override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

	void RunAI();
	void StopAI();

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;

};
