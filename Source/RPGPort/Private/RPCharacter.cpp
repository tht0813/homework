// Fill out your copyright notice in the Description page of Project Settings.


#include "RPCharacter.h"
#include "RPAnimInstance.h"
#include "RPWeapon.h"
#include "RPCharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "RPCharacterWidget.h"
#include "RPAIController.h"
#include "RPCharacterSetting.h"
#include "RPGameInstance.h"
#include "RPPlayerController.h"
#include "RPPlayerState.h"
#include "RPHUDWidget.h"
#include "RPGameMode.h"

// Sets default values
ARPCharacter::ARPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<URPCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	
	
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_GOLDEN(TEXT("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Golden.SK_CharM_Golden'"));
	if (SK_GOLDEN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_GOLDEN.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> PLAYER_ANIM(TEXT("AnimBlueprint'/Game/My/Animation/Player_AnimBP.Player_AnimBP_C'"));
	if (PLAYER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(PLAYER_ANIM.Class);
	}

	SetControlMode(CurrentControlMode);

	ArmLengthSpeed = 10.0f;
	//GetCharacterMovement()->MaxWalkSpeed = 2000.0f;

	GetCharacterMovement()->JumpZVelocity = 800.0f;

	IsAttacking = false;

	MaxCombo = 4;
	AttackEndComboState();

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	AttackRange = 80.0f;
	AttackRadius = 50.0f;

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/My/UI/WBP_HPBar.WBP_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	AIControllerClass = ARPAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AssetIndex = 4;

	SetActorHiddenInGame(true);
	HPBarWidget->SetHiddenInGame(true);
	bCanBeDamaged = false;

	DeadTimer = 5.0f;
}

void ARPCharacter::SetCharacterState(ECharacterState NewState)
{
	RPCHECK(CurrentState != NewState);
	CurrentState = NewState;

	switch(NewState)
	{
	case ECharacterState::LOADING:
		{
		if (bIsPlayer)
		{
			DisableInput(RPPlayerController);

			RPPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);

			auto RPPlayerState = Cast<ARPPlayerState>(GetPlayerState());
			RPCHECK(nullptr != RPPlayerState);
			CharacterStat->SetNewLevel(RPPlayerState->GetCharacterLevel());
		}
		else
		{
			auto RPGameMode = Cast<ARPGameMode>(GetWorld()->GetAuthGameMode());
			RPCHECK(nullptr != RPGameMode);

			int32 TargetLevel = FMath::CeilToInt(((float)RPGameMode->GetScore()*0.8f));
			int32 FinalLevel = FMath::Clamp<int32>(TargetLevel, 1, 20);
			RPLOG(Warning, TEXT("NPC Level : %d"), FinalLevel);
			CharacterStat->SetNewLevel(FinalLevel);

		}

		SetActorHiddenInGame(true);
		HPBarWidget->SetHiddenInGame(true);
		bCanBeDamaged = false;
		break;
		}
	case ECharacterState::READY:
		{
		SetActorHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(false);
		bCanBeDamaged = true;

		CharacterStat->OnHPIsZero.AddLambda([this]() -> void { SetCharacterState(ECharacterState::DEAD); });

		auto CharacterWidget = Cast<URPCharacterWidget>(HPBarWidget->GetUserWidgetObject());
		RPCHECK(nullptr != CharacterWidget);

		CharacterWidget->BindCharacterStat(CharacterStat);

		if (bIsPlayer)
		{
			SetControlMode(EControlMode::Base);
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			EnableInput(RPPlayerController);
		}
		else
		{
			SetControlMode(EControlMode::NPC);
			GetCharacterMovement()->MaxWalkSpeed = 300.0f;
			RPAIController->RunAI();
		}

		break;
		}
	case ECharacterState::DEAD:
		{
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(true);
		RPAnim->SetDeadAnim();
		bCanBeDamaged = false;

		if (bIsPlayer)
		{
			DisableInput(RPPlayerController);
		}
		else
		{
			RPAIController->StopAI();
		}

		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle,
			FTimerDelegate::CreateLambda([this]() -> void {
			if (bIsPlayer)
			{
				RPPlayerController->ShowResultUI();
			}
			else
			{
				Destroy();
			}
		}), DeadTimer, false);
		break;
		}
	}
}

ECharacterState ARPCharacter::GetCharacterState() const
{
	return CurrentState;
}

// Called when the game starts or when spawned
void ARPCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto CharacterWidget = Cast<URPCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}

	bIsPlayer = IsPlayerControlled();

	if (bIsPlayer)
	{
		RPPlayerController = Cast<ARPPlayerController>(GetController());
		RPCHECK(nullptr != RPPlayerController);
	}
	else
	{
		RPAIController = Cast<ARPAIController>(GetController());
		RPCHECK(nullptr != RPAIController);
	}

	auto DefaultSetting = GetDefault<URPCharacterSetting>();

	if (bIsPlayer)
	{
		auto RPPlayerState = Cast<ARPPlayerState>(GetPlayerState());
		RPCHECK(nullptr != RPPlayerState);

		AssetIndex = RPPlayerState->GetCharacterIndex();
	}
	else
	{
		AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	}

	CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];

	auto RPGameInstance = Cast<URPGameInstance>(GetGameInstance());
	RPCHECK(nullptr != RPGameInstance);

	AssetStreamingHandle = RPGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad,
		FStreamableDelegate::CreateUObject(this, &ARPCharacter::OnAssetLoadCompleted));
	SetCharacterState(ECharacterState::LOADING);
}

void ARPCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case EControlMode::Base:
		SpringArm->SetRelativeLocation(FVector::ZeroVector);
		ArmLengthTo = 500.0f;
		break;
	}
	switch (CurrentControlMode)
	{
	case EControlMode::Zoom:
		SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
		ArmLengthTo = -50.0f;
		break;
	}
	switch (CurrentControlMode)
	{
	case EControlMode::NPC:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	}
}

// Called every frame
void ARPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);
}

void ARPCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	RPAnim = Cast<URPAnimInstance>(GetMesh()->GetAnimInstance());
	RPCHECK(nullptr != RPAnim);

	RPAnim->OnMontageEnded.AddDynamic(this, &ARPCharacter::OnAttackMontageEnded);

	RPAnim->OnNextAttackCheck.AddLambda([this]()->void{
		RPLOG(Log, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			RPAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	RPAnim->OnAttackHitCheck.AddUObject(this, &ARPCharacter::AttackCheck);

	CharacterStat->OnHPIsZero.AddLambda([this]() -> void{
		RPLOG(Warning, TEXT("HP Zero"));
		RPAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	});
}

float ARPCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	RPLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	CharacterStat->SetDamage(FinalDamage);

	if (CurrentState == ECharacterState::DEAD)
	{
		if (EventInstigator->IsPlayerController())
		{
			auto RPPlayerCTL = Cast<ARPPlayerController>(EventInstigator);
			RPCHECK(nullptr != RPPlayerCTL, 0.0f);
			RPPlayerCTL->NPCKill(this);
		}
	}

	return FinalDamage;
}

void ARPCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		SetControlMode(EControlMode::Base);
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else
	{
		SetControlMode(EControlMode::NPC);
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}

// Called to bind functionality to input
void ARPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &ARPCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ARPCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ARPCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("UsePotion"), EInputEvent::IE_Pressed, this, &ARPCharacter::UsePotion);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ARPCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ARPCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ARPCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ARPCharacter::Turn);
}

bool ARPCharacter::CanSetWeapon()
{
	return true;
}

void ARPCharacter::SetWeapon(ARPWeapon* NewWeapon)
{
	RPCHECK(nullptr != NewWeapon);
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	FName WeaponSocket(TEXT("hand_rSocket"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

void ARPCharacter::UpDown(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}

void ARPCharacter::LeftRight(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}

void ARPCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void ARPCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void ARPCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
	case EControlMode::Base:
		SetControlMode(EControlMode::Zoom);
		break;
	case EControlMode::Zoom:
		SetControlMode(EControlMode::Base);
		break;

	}
}

void ARPCharacter::UsePotion()
{
	if (CharacterStat->GetCurrentHP() == CharacterStat->GetMaxHP())
	{
		RPLOG(Warning, TEXT("Full HP"));
		return;
	}

	auto RPPlayerState = Cast<ARPPlayerState>(GetPlayerState());
	RPCHECK(nullptr != RPPlayerState);
	RPPlayerState->SubPotionCount();

	if (RPPlayerState->GetPotionCount() <= 0)
	{
		return;
	}

	CharacterStat->SetHP(CharacterStat->GetMaxHP());
}

void ARPCharacter::Attack()
{
	if (IsAttacking)
	{
		RPCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		RPCHECK(CurrentCombo == 0);
		AttackStartComboState();
		RPAnim->PlayAttackMontage();
		RPAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

int32 ARPCharacter::GetExp() const
{
	return CharacterStat->GetDropExp();
}

void ARPCharacter::UpdateStat()
{
	auto RPPlayerState = Cast<ARPPlayerState>(GetPlayerState());
	RPCHECK(nullptr != RPPlayerState);

	CharacterStat->SetNewLevel(RPPlayerState->GetCharacterLevel());
}

float ARPCharacter::GetFinalAttackRange() const
{
	return (nullptr!=CurrentWeapon) ? CurrentWeapon->GetAttackRange() : AttackRange;
}

float ARPCharacter::GetFinalAttackDamage() const
{
	float AttackDamage = (nullptr != CurrentWeapon) ? (CharacterStat->GetAttack() + CurrentWeapon->GetAttackDamage())
		: (CharacterStat->GetAttack());
	float AttackModifier = (nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackModifier() : 1.0f;

	return AttackDamage * AttackModifier;
}

void ARPCharacter::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	RPCHECK(IsAttacking);
	RPCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
	OnAttackEnd.Broadcast();
}

void ARPCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	RPCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void ARPCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void ARPCharacter::AttackCheck()
{
	float FinalAttackRange = GetFinalAttackRange();

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * FinalAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * FinalAttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = FinalAttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			RPLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(GetFinalAttackDamage(), DamageEvent, GetController(), this);
		}
	}
}

void ARPCharacter::OnAssetLoadCompleted()
{
	AssetStreamingHandle->ReleaseHandle();
	TSoftObjectPtr<USkeletalMesh> LoadedAssetPath(CharacterAssetToLoad);
	RPCHECK(LoadedAssetPath.IsValid());

	GetMesh()->SetSkeletalMesh(LoadedAssetPath.Get());
	SetCharacterState(ECharacterState::READY);
}
