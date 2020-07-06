// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGPort.h"
#include "GameFramework/GameStateBase.h"
#include "RPGameState.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORT_API ARPGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ARPGameState();

	int32 GetTotalGameScore() const;
	void AddGameScore();

	void SetGameCleared();
	bool IsGameCleared() const;

private:
	UPROPERTY()
	int32 TotalGameScore;

	UPROPERTY()
	bool bGameCleared;
};
