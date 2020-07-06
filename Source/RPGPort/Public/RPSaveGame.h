// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGPort.h"
#include "GameFramework/SaveGame.h"
#include "RPSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORT_API URPSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	URPSaveGame();

	UPROPERTY()
	int32 Level;

	UPROPERTY()
	int32 Exp;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 HighScore;

	UPROPERTY()
	int32 CharacterIndex;

};
