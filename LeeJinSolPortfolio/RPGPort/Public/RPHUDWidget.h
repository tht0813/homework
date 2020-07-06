// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGPort.h"
#include "Blueprint/UserWidget.h"
#include "RPHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORT_API URPHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(class URPCharacterStatComponent* CharacterStat);
	void BindPlayerState(class ARPPlayerState* PlayerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();

private:
	TWeakObjectPtr<class URPCharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class ARPPlayerState> CurrentPlayerState;

	UPROPERTY()
	class UProgressBar* HPBar;

	UPROPERTY()
	class UProgressBar* ExpBar;

	UPROPERTY()
	class UTextBlock* PlayerName;

	UPROPERTY()
	class UTextBlock* PlayerLevel;

	UPROPERTY()
	class UTextBlock* CurrentScore;

	UPROPERTY()
	class UTextBlock* HighScore;

	UPROPERTY()
	class UTextBlock* PotionCount;
};
