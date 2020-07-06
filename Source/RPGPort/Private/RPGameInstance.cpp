// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGameInstance.h"

URPGameInstance::URPGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/My/GameData/RPCharacterData.RPCharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_RPCHARACTER(*CharacterDataPath);
	RPCHECK(DT_RPCHARACTER.Succeeded());
	RPCharacterTable = DT_RPCHARACTER.Object;
	RPCHECK(RPCharacterTable->GetRowMap().Num() > 0);
}

void URPGameInstance::Init()
{
	Super::Init();

}

FRPCharacterData* URPGameInstance::GetRPCharacterData(int32 Level)
{
	return RPCharacterTable->FindRow<FRPCharacterData>(*FString::FromInt(Level), TEXT(""));
}