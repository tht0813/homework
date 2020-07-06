// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPCharacterSetting.generated.h"

/**
 * 
 */
UCLASS(config=RPGPort)
class RPGPORTSETTING_API URPCharacterSetting : public UObject
{
	GENERATED_BODY()
	
public:
	URPCharacterSetting();

	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets;
};
