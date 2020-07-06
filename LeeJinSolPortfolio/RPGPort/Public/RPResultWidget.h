// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGPort.h"
#include "RPGameplayWidget.h"
#include "RPResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORT_API URPResultWidget : public URPGameplayWidget
{
	GENERATED_BODY()
	
public:
	void BindGameState(class ARPGameState* GameState);
	
protected:
	virtual void NativeConstruct() override;

private:
	TWeakObjectPtr<class ARPGameState> CurrentGameState;

};
