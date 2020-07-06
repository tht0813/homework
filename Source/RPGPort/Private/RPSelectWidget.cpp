// Fill out your copyright notice in the Description page of Project Settings.


#include "RPSelectWidget.h"
#include "RPCharacterSetting.h"
#include "RPGameInstance.h"
#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "RPSaveGame.h"
#include "RPPlayerState.h"

void URPSelectWidget::NextCharacter(bool bForward)
{
	bForward ? CurrentIndex++ : CurrentIndex--;

	if (CurrentIndex == -1) CurrentIndex = MaxIndex = 1;
	if (CurrentIndex == MaxIndex) CurrentIndex = 0;

	auto CharacterSetting = GetDefault<URPCharacterSetting>();
	auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];

	auto RPGameInstance = GetWorld()->GetGameInstance<URPGameInstance>();
	RPCHECK(nullptr != RPGameInstance);
	
	RPCHECK(TargetComponent.IsValid());

	USkeletalMesh* Asset = RPGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);
	if (nullptr != Asset)
	{
		TargetComponent->SetSkeletalMesh(Asset);
	}
}

void URPSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentIndex = 0;
	auto CharacterSetting = GetDefault<URPCharacterSetting>();
	MaxIndex = CharacterSetting->CharacterAssets.Num();

	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)
	{
		TargetComponent = It->GetSkeletalMeshComponent();
		break;
	}

	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	RPCHECK(nullptr != PrevButton);

	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	RPCHECK(nullptr != NextButton);

	TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	RPCHECK(nullptr != TextBox);

	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
	RPCHECK(nullptr != ConfirmButton);

	PrevButton->OnClicked.AddDynamic(this, &URPSelectWidget::OnPrevClicked);
	NextButton->OnClicked.AddDynamic(this, &URPSelectWidget::OnNextClicked);
	ConfirmButton->OnClicked.AddDynamic(this, &URPSelectWidget::OnConfirmClicked);
}

void URPSelectWidget::OnPrevClicked()
{
	NextCharacter(false);
}

void URPSelectWidget::OnNextClicked()
{
	NextCharacter(true);
}

void URPSelectWidget::OnConfirmClicked()
{
	FString CharacterName = TextBox->GetText().ToString();
	if(CharacterName.Len() <= 0 || (CharacterName.Len() > 10)) return;

	URPSaveGame* NewPlayerData = NewObject<URPSaveGame>();
	NewPlayerData->PlayerName = CharacterName;
	NewPlayerData->Level = 1;
	NewPlayerData->Exp = 0;
	NewPlayerData->HighScore = 0;
	NewPlayerData->CharacterIndex = CurrentIndex;

	auto RPPlayerState = GetDefault<ARPPlayerState>();
	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, RPPlayerState->SaveSlotName, 0))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Test"));
	}
	else
	{
		RPLOG(Error, TEXT("Save Error"));
	}
}
