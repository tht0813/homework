// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGPort.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "RPGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FRPCharacterData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FRPCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;

};


/**
 * 
 */
UCLASS()
class RPGPORT_API URPGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	URPGameInstance();

	virtual void Init() override;
	FRPCharacterData* GetRPCharacterData(int32 Level);

	FStreamableManager StreamableManager;

private:
	UPROPERTY()
	class UDataTable* RPCharacterTable;

};
