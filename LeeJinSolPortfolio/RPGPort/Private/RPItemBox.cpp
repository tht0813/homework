// Fill out your copyright notice in the Description page of Project Settings.


#include "RPItemBox.h"
#include "RPWeapon.h"
#include "RPCharacter.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ARPItemBox::ARPItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("StaticMesh'/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}

	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_OPENBOX(TEXT("ParticleSystem'/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (P_OPENBOX.Succeeded())
	{
		Effect->SetTemplate(P_OPENBOX.Object);
		Effect->bAutoActivate = false;
	}

	Effect->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));

	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	WeaponItemClass = ARPWeapon::StaticClass();

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_GETWEAPON_C(TEXT("/Game/My/UI/UI_GetWeapon.UI_GetWeapon_C"));
	if (UI_GETWEAPON_C.Succeeded())
	{
		GetWeaponWidgetClass = UI_GETWEAPON_C.Class;
	}

	WidgetOpenTime = 1.0f;
}

// Called when the game starts or when spawned
void ARPItemBox::BeginPlay()
{
	Super::BeginPlay();
	
	GetWeaponWidgetInstance = CreateWidget<UUserWidget>(GetWorld()->GetGameInstance(), GetWeaponWidgetClass);
	RPCHECK(nullptr != GetWeaponWidgetInstance);
}

void ARPItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ARPItemBox::OnCharacterOverlap);
}

void ARPItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	RPLOG_S(Warning);

	auto RPCharacter = Cast<ARPCharacter>(OtherActor);
	RPCHECK(nullptr != RPCharacter);

	if (nullptr != RPCharacter && nullptr != WeaponItemClass)
	{
		if (RPCharacter->CanSetWeapon())
		{
			auto NewWeapon = GetWorld()->SpawnActor<ARPWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			RPCharacter->SetWeapon(NewWeapon);

			Effect->Activate(true);
			Box->SetHiddenInGame(true, true);
			SetActorEnableCollision(false);
			Effect->OnSystemFinished.AddDynamic(this, &ARPItemBox::OnEffectFinished);
			
			GetWeaponWidgetInstance->AddToViewport();

			GetWorld()->GetTimerManager().SetTimer(
				WeaponWidgetTimerHandle,
				FTimerDelegate::CreateLambda([this]() -> void {
				GetWeaponWidgetInstance->RemoveFromParent();
				}),
				WidgetOpenTime,
				false
				);
		}
		else
		{
			RPLOG(Warning, TEXT("%s can't equip weapon."), *RPCharacter->GetName());
		}
	}
}

void ARPItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}
