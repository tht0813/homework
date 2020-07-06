// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGPort.h"
#include "GameFramework/GameModeBase.h"
#include "RPGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORT_API ARPGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	ARPGameMode();
	
public:
	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	void AddScore(class ARPPlayerController* ScoredPlayer);

	int GetScore() const;

private:
	UPROPERTY()
	class ARPGameState* RPGameState;

	UPROPERTY()
	int32 ScoreToClear;

};
