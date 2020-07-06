// Fill out your copyright notice in the Description page of Project Settings.


#include "RPPlayerController.h"
#include "RPHUDWidget.h"
#include "RPPlayerState.h"
#include "RPCharacter.h"
#include "RPGameplayWidget.h"
#include "RPResultWidget.h"
#include "RPGameState.h"

ARPPlayerController::ARPPlayerController()
{
	static ConstructorHelpers::FClassFinder<URPHUDWidget> UI_HUD_C(TEXT("/Game/My/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}

	static ConstructorHelpers::FClassFinder<URPGameplayWidget> UI_MENU_C(TEXT("/Game/My/UI/UI_Menu.UI_Menu_C"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}

	static ConstructorHelpers::FClassFinder<URPGameplayWidget> UI_RESULT_C(TEXT("/Game/My/UI/UI_Result.UI_Result_C"));
	if (UI_RESULT_C.Succeeded())
	{
		ResultWidgetClass = UI_RESULT_C.Class;
	}
}

void ARPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ChangeInputMode(true);

	HUDWidget = CreateWidget<URPHUDWidget>(this, HUDWidgetClass);
	RPCHECK(nullptr != HUDWidget);
	HUDWidget->AddToViewport(1);

	ResultWidget = CreateWidget<URPResultWidget>(this, ResultWidgetClass);
	RPCHECK(nullptr != ResultWidget);

	RPPlayerState = Cast<ARPPlayerState>(PlayerState);
	RPCHECK(nullptr != RPPlayerState);

	HUDWidget->BindPlayerState(RPPlayerState);
	RPPlayerState->OnPlayerStateChanged.Broadcast();
}

void ARPPlayerController::OnGamePause()
{
	MenuWidget = CreateWidget<URPGameplayWidget>(this, MenuWidgetClass);
	RPCHECK(nullptr != MenuWidget);
	MenuWidget->AddToViewport(3);

	SetPause(true);
	ChangeInputMode(false);
}

void ARPPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ARPPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void ARPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &ARPPlayerController::OnGamePause);
}

URPHUDWidget * ARPPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void ARPPlayerController::NPCKill(class ARPCharacter* KilledNPC) const
{
	bool bLevelUp = RPPlayerState->AddExp(KilledNPC->GetExp());
	if (bLevelUp)
	{
		ARPCharacter* RPCharacter = Cast<ARPCharacter>(GetCharacter());
		RPCHECK(nullptr != RPCharacter);
		RPCharacter->UpdateStat();
	}
}

void ARPPlayerController::AddGameScore() const
{
	RPPlayerState->AddGameScore();
}

void ARPPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void ARPPlayerController::ShowResultUI()
{
	auto RPGameState = Cast<ARPGameState>(UGameplayStatics::GetGameState(this));
	RPCHECK(nullptr != RPGameState);
	ResultWidget->BindGameState(RPGameState);

	ResultWidget->AddToViewport();
	ChangeInputMode(false);
}
