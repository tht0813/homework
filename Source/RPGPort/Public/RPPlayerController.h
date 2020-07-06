// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGPort.h"
#include "GameFramework/PlayerController.h"
#include "RPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORT_API ARPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARPPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;

	class URPHUDWidget* GetHUDWidget() const;
	void NPCKill(class ARPCharacter* KilledNPC) const;

	void AddGameScore() const;

	void ChangeInputMode(bool bGameMode = true);

	void ShowResultUI();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class URPHUDWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class URPGameplayWidget> MenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class URPResultWidget> ResultWidgetClass;

private:
	void OnGamePause();

	UPROPERTY()
	class URPHUDWidget* HUDWidget;

	UPROPERTY()
	class ARPPlayerState* RPPlayerState;

	UPROPERTY()
	class URPGameplayWidget* MenuWidget;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;

	UPROPERTY()
	class URPResultWidget* ResultWidget;

};
