// Fill out your copyright notice in the Description page of Project Settings.


#include "RPCharacterWidget.h"
#include "RPCharacterStatComponent.h"
#include "Components/ProgressBar.h"


void URPCharacterWidget::BindCharacterStat(URPCharacterStatComponent* NewCharacterStat)
{
	RPCHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &URPCharacterWidget::UpdateHPWidget);
}

void URPCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	RPCHECK(nullptr != HPProgressBar);
	UpdateHPWidget();
}

void URPCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}