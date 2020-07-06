// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGPort.h"
#include "GameFramework/PlayerState.h"
#include "RPPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangeDelegate);

/**
 * 
 */
UCLASS()
class RPGPORT_API ARPPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ARPPlayerState();

	int32 GetGameScore() const;
	int32 GetGameHighScore() const;
	FString SaveSlotName;

	int32 GetCharacterLevel() const;
	int32 GetCharacterIndex() const;
	int32 GetPotionCount() const;
	float GetExpRatio() const;
	bool AddExp(int32 InExp);
	void AddGameScore();
	void SubPotionCount();

	void InitPlayerData();
	void SavePlayerData();

	FOnPlayerStateChangeDelegate OnPlayerStateChanged;

protected:
	UPROPERTY(Transient)
	int32 GameScore;

	UPROPERTY(Transient)
	int32 CharacterLevel;

	UPROPERTY(Transient)
	int32 Exp;

	UPROPERTY(Transient)
	int32 GameHighScore;

	UPROPERTY(Transient)
	int32 CharacterIndex;

	UPROPERTY(Transient)
	int32 PotionCount;

private:
	void SetCharacterLevel(int32 NewCharacterLevel);
	struct FRPCharacterData* CurrentStatData;

};
