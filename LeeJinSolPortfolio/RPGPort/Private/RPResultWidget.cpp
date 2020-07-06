// Fill out your copyright notice in the Description page of Project Settings.


#include "RPResultWidget.h"
#include "Components/TextBlock.h"
#include "RPGameState.h"

void URPResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
	RPCHECK(nullptr != Result);
	Result->SetText(FText::FromString(CurrentGameState->IsGameCleared() ?
	TEXT("Mission Clear") : TEXT("Mission Failed")));

	auto TotalScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));
	RPCHECK(nullptr != TotalScore);
	TotalScore->SetText(FText::FromString(FString::FromInt(CurrentGameState->GetTotalGameScore())));
}

void URPResultWidget::BindGameState(class ARPGameState* GameState)
{
	RPCHECK(nullptr != GameState);
	CurrentGameState = GameState;
}
