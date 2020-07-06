// Fill out your copyright notice in the Description page of Project Settings.


#include "RPCharacterStatComponent.h"
#include "RPGameInstance.h"

// Sets default values for this component's properties
URPCharacterStatComponent::URPCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	Level = 1;
}


// Called when the game starts
void URPCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URPCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void URPCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto RPGameInstance = Cast<URPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	RPCHECK(nullptr != RPGameInstance);
	
	CurrentStatData = RPGameInstance->GetRPCharacterData(NewLevel);
	
	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
	}
	else
	{
		RPLOG(Error, TEXT("Level (%d) data doesn't exist"), NewLevel);
	}
}

void URPCharacterStatComponent::SetDamage(float NewDamage)
{
	RPCHECK(nullptr != CurrentStatData);

	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

void URPCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	
	if (CurrentHP <= KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

float URPCharacterStatComponent::GetAttack()
{
	RPCHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}

float URPCharacterStatComponent::GetHPRatio()
{
	RPCHECK(nullptr != CurrentStatData, 0.0f);

	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}

int32 URPCharacterStatComponent::GetDropExp() const
{
	return CurrentStatData->DropExp;
}

float URPCharacterStatComponent::GetMaxHP() const
{
	return CurrentStatData->MaxHP;
}

float URPCharacterStatComponent::GetCurrentHP() const
{
	return CurrentHP;
}
