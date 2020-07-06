// Fill out your copyright notice in the Description page of Project Settings.


#include "RPUIPlayerController.h"
#include "Blueprint/UserWidget.h"

void ARPUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	RPCHECK(nullptr != UIWidgetClass);

	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	RPCHECK(nullptr != UIWidgetInstance);

	UIWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	//Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());

	SetInputMode(Mode);
	bShowMouseCursor = true;
}
