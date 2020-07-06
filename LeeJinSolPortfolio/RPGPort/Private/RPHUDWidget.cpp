// Fill out your copyright notice in the Description page of Project Settings.


#include "RPHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "RPCharacterStatComponent.h"
#include "RPPlayerState.h"

void URPHUDWidget::BindCharacterStat(class URPCharacterStatComponent* CharacterStat)
{
	RPCHECK(nullptr != CharacterStat);
	CurrentCharacterStat = CharacterStat;
	CharacterStat->OnHPChanged.AddUObject(this, &URPHUDWidget::UpdateCharacterStat);
}

void URPHUDWidget::BindPlayerState(class ARPPlayerState* PlayerState)
{
	RPCHECK(nullptr != PlayerState);
	CurrentPlayerState = PlayerState;
	PlayerState->OnPlayerStateChanged.AddUObject(this, &URPHUDWidget::UpdatePlayerState);
}

void URPHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	RPCHECK(nullptr != HPBar);

	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbExp")));
	RPCHECK(nullptr != ExpBar);

	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerName")));
	RPCHECK(nullptr != PlayerName);

	PlayerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLevel")));
	RPCHECK(nullptr != PlayerLevel);

	CurrentScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentScore")));
	RPCHECK(nullptr != CurrentScore);

	HighScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtHighScore")));
	RPCHECK(nullptr != HighScore);

	PotionCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPotionCount")));
	RPCHECK(nullptr != PotionCount);
}

void URPHUDWidget::UpdateCharacterStat()
{
	RPCHECK(CurrentCharacterStat.IsValid());

	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
}

void URPHUDWidget::UpdatePlayerState()
{
	RPCHECK(CurrentPlayerState.IsValid());

	ExpBar->SetPercent(CurrentPlayerState->GetExpRatio());

	PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));

	PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));

	CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameScore())));

	HighScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameHighScore())));

	PotionCount->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetPotionCount())));
}
