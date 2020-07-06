// Fill out your copyright notice in the Description page of Project Settings.


#include "RPPlayerState.h"
#include "RPGameInstance.h"
#include "RPSaveGame.h"

ARPPlayerState::ARPPlayerState()
{
	GameScore = 0;
	GameHighScore = 0;
	CharacterLevel = 1;
	Exp = 0;
	SaveSlotName = TEXT("Test1");
	CharacterIndex = 0;
	PotionCount = 5;
}

int32 ARPPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 ARPPlayerState::GetGameHighScore() const
{
	return GameHighScore;
}

int32 ARPPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

int32 ARPPlayerState::GetCharacterIndex() const
{
	return CharacterIndex;
}

int32 ARPPlayerState::GetPotionCount() const
{
	return PotionCount;
}

float ARPPlayerState::GetExpRatio() const
{
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER) return 0.0f;

	float Result = (float)Exp / (float)CurrentStatData->NextExp;
	return Result;
}

bool ARPPlayerState::AddExp(int32 InExp)
{
	if (CurrentStatData->NextExp == -1) return false;

	bool bLevelUp = false;
	Exp += InExp;
	if (Exp >= CurrentStatData->NextExp)
	{
		Exp -= CurrentStatData->NextExp;
		SetCharacterLevel(CharacterLevel + 1);
		bLevelUp = true;
	}

	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
	return bLevelUp;
}

void ARPPlayerState::AddGameScore()
{
	GameScore++;
	if (GameScore >= GameHighScore)
	{
		GameHighScore = GameScore;
	}
	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
}

void ARPPlayerState::SubPotionCount()
{
	if (PotionCount <= 0) 
	{ 
		RPLOG(Warning, TEXT("No Potion")); 
		return;
	}
	PotionCount--;
	OnPlayerStateChanged.Broadcast();
}

void ARPPlayerState::InitPlayerData()
{
	auto RPSaveGame = Cast<URPSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == RPSaveGame)
	{
		RPSaveGame = GetMutableDefault<URPSaveGame>();
	}

	SetPlayerName(RPSaveGame->PlayerName);
	SetCharacterLevel(RPSaveGame->Level);
	GameScore = 0;
	GameHighScore = RPSaveGame->HighScore;
	Exp = RPSaveGame->Exp;
	CharacterIndex = RPSaveGame->CharacterIndex;
	SavePlayerData();
}

void ARPPlayerState::SavePlayerData()
{
	URPSaveGame*  NewPlayerData = NewObject<URPSaveGame>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->HighScore = GameHighScore;
	NewPlayerData->CharacterIndex = CharacterIndex;

	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		RPLOG(Error, TEXT("Save Error"));
	}
}

void ARPPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto RPGameInstance = Cast<URPGameInstance>(GetGameInstance());
	RPCHECK(nullptr != RPGameInstance);

	CurrentStatData = RPGameInstance->GetRPCharacterData(NewCharacterLevel);
	RPCHECK(nullptr != CurrentStatData);

	CharacterLevel = NewCharacterLevel;
}
