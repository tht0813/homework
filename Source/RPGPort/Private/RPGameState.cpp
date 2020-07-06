// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGameState.h"

ARPGameState::ARPGameState()
{
	TotalGameScore = 0;
	bGameCleared = false;
}

int32 ARPGameState::GetTotalGameScore() const
{
	return TotalGameScore;
}

void ARPGameState::AddGameScore()
{
	TotalGameScore++;
}

void ARPGameState::SetGameCleared()
{
	bGameCleared = true;
}

bool ARPGameState::IsGameCleared() const
{
	return bGameCleared;
}