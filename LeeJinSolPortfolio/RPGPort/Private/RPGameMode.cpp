// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGameMode.h"
#include "RPCharacter.h"
#include "RPPlayerController.h"
#include "RPPlayerState.h"
#include "RPGameState.h"

ARPGameMode::ARPGameMode()
{
	DefaultPawnClass = ARPCharacter::StaticClass();
	PlayerControllerClass = ARPPlayerController::StaticClass();
	PlayerStateClass = ARPPlayerState::StaticClass();
	GameStateClass = ARPGameState::StaticClass();

	ScoreToClear = 3;
}

void ARPGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RPGameState = Cast<ARPGameState>(GameState);
}

void ARPGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto RPPlayerState = Cast<ARPPlayerState>(NewPlayer->PlayerState);
	RPCHECK(nullptr != RPPlayerState);
	RPPlayerState->InitPlayerData();
}

void ARPGameMode::AddScore(ARPPlayerController * ScoredPlayer)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto RPPlayerController = Cast<ARPPlayerController>(It->Get());
		if ((nullptr != RPPlayerController) && (ScoredPlayer == RPPlayerController))
		{
			RPPlayerController->AddGameScore();
			break;
		}
	}

	RPGameState->AddGameScore();

	if (GetScore() >= ScoreToClear)
	{
		RPGameState->SetGameCleared();

		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			(*It)->TurnOff();
		}

		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			const auto RPPlayerController = Cast<ARPPlayerController>(It->Get());
			if (nullptr != RPPlayerController)
			{
				RPPlayerController->ShowResultUI();
			}
		}
	}
}

int32 ARPGameMode::GetScore() const
{
	return RPGameState->GetTotalGameScore();
}
