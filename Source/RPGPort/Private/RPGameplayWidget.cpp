// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGameplayWidget.h"
#include "Components/Button.h"
#include "RPPlayerController.h"

void URPGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &URPGameplayWidget::OnResumeClicked);
	}

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &URPGameplayWidget::OnReturnToTitleClicked);
	}

	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (nullptr != RetryGameButton)
	{
		RetryGameButton->OnClicked.AddDynamic(this, &URPGameplayWidget::OnRetryGameClicked);
	}
}

void URPGameplayWidget::OnResumeClicked()
{
	auto RPPlayerController = Cast<ARPPlayerController>(GetOwningPlayer());
	RPCHECK(nullptr != RPPlayerController);

	RemoveFromParent();
	RPPlayerController->ChangeInputMode(true);
	RPPlayerController->SetPause(false);
}

void URPGameplayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}

void URPGameplayWidget::OnRetryGameClicked()
{
	auto RPPlayerController = Cast<ARPPlayerController>(GetOwningPlayer());
	RPCHECK(nullptr != RPPlayerController);
	RPPlayerController->RestartLevel();
}
